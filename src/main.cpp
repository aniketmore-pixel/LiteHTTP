#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <filesystem>
#include <fstream>
#include <cstring>
#include <cerrno>
#include <csignal>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/sendfile.h>
#include <sys/stat.h>


#include "thread_pool.h"
#include "http.h"
#include "cache.h"


namespace fs = std::filesystem;


static volatile std::sig_atomic_t running = 1;
void handle_sigint(int){ running = 0; }


int make_socket_non_blocking(int sfd){
int flags = fcntl(sfd, F_GETFL, 0);
if (flags == -1) return -1;
return fcntl(sfd, F_SETFL, flags | O_NONBLOCK);
}


int create_and_bind(uint16_t port){
int sfd = ::socket(AF_INET, SOCK_STREAM, 0);
if (sfd==-1) return -1;
int opt = 1; setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
sockaddr_in addr{}; addr.sin_family = AF_INET; addr.sin_addr.s_addr = htonl(INADDR_ANY); addr.sin_port = htons(port);
if (bind(sfd, (sockaddr*)&addr, sizeof(addr))==-1) { close(sfd); return -1; }
if (listen(sfd, 1024)==-1) { close(sfd); return -1; }
return sfd;
}


std::string mime_type(const std::string &path){
if (path.ends_with(".html")) return "text/html";
if (path.ends_with(".css")) return "text/css";
if (path.ends_with(".js")) return "application/javascript";
if (path.ends_with(".png")) return "image/png";
if (path.ends_with(".jpg") || path.ends_with(".jpeg")) return "image/jpeg";
return "text/plain";
}


int main(int argc, char** argv){
uint16_t port = 8080; if (argc>1) port = static_cast<uint16_t>(std::stoi(argv[1]));
std::string doc_root = "www"; if (argc>2) doc_root = argv[2];


std::signal(SIGINT, handle_sigint);


int sfd = create_and_bind(port);
if (sfd==-1){ std::cerr << "Failed to bind: " << std::strerror(errno) << "\n"; return 1; }
make_socket_non_blocking(sfd);


int efd = epoll_create1(0);
if (efd==-1){ std::cerr << "epoll_create1 failed\n"; return 1; }


epoll_event event{}; event.data.fd = sfd; event.events = EPOLLIN;
}