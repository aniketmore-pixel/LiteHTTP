#include <iostream>
#include <string>
#include <csignal>
#include <cstring>
#include <vector>
#include <filesystem>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fstream>
#include <mutex>
#include <chrono>
#include <iomanip>

#include "http.h"
#include "thread_pool.h"
#include "cache.h"

namespace fs = std::filesystem;

volatile bool running = true;
std::mutex log_mu;

// Thread-safe timestamped log
void log(const std::string &msg, const std::string &color = "\033[0m") {
    std::lock_guard<std::mutex> lock(log_mu);
    auto now = std::chrono::system_clock::now();
    auto itt = std::chrono::system_clock::to_time_t(now);
    std::cout << color << "[" << std::put_time(localtime(&itt), "%H:%M:%S") << "] " 
              << msg << "\033[0m" << std::endl;
}

void handle_sigint(int) {
    running = false;
    log("🛑 Caught SIGINT. Shutting down...", "\033[1;31m");
}

int make_socket_non_blocking(int sfd) {
    int flags = fcntl(sfd, F_GETFL, 0);
    if (flags == -1) return -1;
    return fcntl(sfd, F_SETFL, flags | O_NONBLOCK);
}

int create_and_bind(uint16_t port) {
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1) return -1;

    int opt = 1;
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(sfd, (sockaddr*)&addr, sizeof(addr)) == -1) {
        close(sfd);
        return -1;
    }

    return sfd;
}

int main(int argc, char** argv) {
    uint16_t port = 8080; 
    if (argc > 1) port = static_cast<uint16_t>(std::stoi(argv[1]));
    std::string doc_root = "www"; 
    if (argc > 2) doc_root = argv[2];

    log("🚀 Starting LiteHTTP on port " + std::to_string(port) + 
        " serving directory: " + doc_root, "\033[1;34m");

    if (!fs::exists(doc_root)) {
        log("❌ ERROR: directory not found -> " + doc_root, "\033[1;31m");
        return 7;
    }

    std::signal(SIGINT, handle_sigint);

    int sfd = create_and_bind(port);
    if (sfd == -1) { 
        log("❌ Failed to bind: " + std::string(std::strerror(errno)), "\033[1;31m");
        return 1; 
    }
    make_socket_non_blocking(sfd);

    if (listen(sfd, SOMAXCONN) == -1) {
        log("❌ listen failed: " + std::string(std::strerror(errno)), "\033[1;31m");
        return 1;
    }

    int efd = epoll_create1(0);
    if (efd == -1) { 
        log("❌ epoll_create1 failed", "\033[1;31m"); 
        return 1; 
    }

    epoll_event event{}; 
    event.data.fd = sfd; 
    event.events = EPOLLIN;

    if (epoll_ctl(efd, EPOLL_CTL_ADD, sfd, &event) == -1) {
        log("❌ epoll_ctl failed: " + std::string(std::strerror(errno)), "\033[1;31m");
        return 1;
    }

    const int MAX_EVENTS = 64;
    std::vector<epoll_event> events(MAX_EVENTS);

    log("✅ Server is running. Press Ctrl+C to stop.", "\033[1;32m");

    ThreadPool pool(4); // 4 worker threads
    LRUCache cache(256*1024); // 256KB cache

    while (running) {
        int n = epoll_wait(efd, events.data(), MAX_EVENTS, 1000);
        if (n == -1) {
            if (errno == EINTR) continue;
            log("❌ epoll_wait failed: " + std::string(std::strerror(errno)), "\033[1;31m");
            break;
        }

        for (int i = 0; i < n; i++) {
            if (events[i].data.fd == sfd) {
                sockaddr_in in_addr{};
                socklen_t in_len = sizeof(in_addr);
                int infd = accept(sfd, (sockaddr*)&in_addr, &in_len);
                if (infd == -1) {
                    if (errno != EAGAIN && errno != EWOULDBLOCK) {
                        log("❌ accept failed: " + std::string(std::strerror(errno)), "\033[1;31m");
                    }
                    continue;
                }

                make_socket_non_blocking(infd);

                epoll_event client_event{};
                client_event.data.fd = infd;
                client_event.events = EPOLLIN | EPOLLET;
                if (epoll_ctl(efd, EPOLL_CTL_ADD, infd, &client_event) == -1) {
                    log("❌ epoll_ctl add client failed: " + std::string(std::strerror(errno)), "\033[1;31m");
                    close(infd);
                    continue;
                }

                log("✅ Accepted new client: fd=" + std::to_string(infd), "\033[1;36m");
            } else {
                int fd = events[i].data.fd;
                char buffer[4096];
                ssize_t count = read(fd, buffer, sizeof(buffer)-1);
                if (count <= 0) {
                    close(fd);
                    continue;
                }
                buffer[count] = '\0';
                std::string raw(buffer);
                HttpRequest req;
                if (!parse_http_request(raw, req)) {
                    close(fd);
                    continue;
                }

                log("📄 Request received: " + req.method + " " + req.path, "\033[1;33m");

                pool.enqueue([fd, req, &doc_root, &cache]{
                    HttpResponse res;
                    std::string path = doc_root + req.path;
                    if (req.path == "/") path += "index.html";

                    std::string content;
                    if (!cache.get(path, content)) {
                        std::ifstream f(path, std::ios::binary);
                        if (f) {
                            content.assign((std::istreambuf_iterator<char>(f)),
                                            std::istreambuf_iterator<char>());
                            cache.put(path, content);
                        } else {
                            res.status = 404;
                            res.status_text = "Not Found";
                            content = "404 Not Found";
                        }
                    }
                    res.body = content;
                    res.headers["Content-Length"] = std::to_string(res.body.size());
                    res.headers["Content-Type"] = "text/html";
                    res.headers["Connection"] = "close";

                    std::string response = build_http_response_header(res) + res.body;
                    write(fd, response.c_str(), response.size());
                    close(fd);

                    log("✅ Response sent to fd=" + std::to_string(fd) +
                        " (status: " + std::to_string(res.status) + ")", "\033[1;32m");
                });
            }
        }
    }

    close(sfd);
    close(efd);

    log("👋 Server stopped.", "\033[1;34m");
    return 0;
}
