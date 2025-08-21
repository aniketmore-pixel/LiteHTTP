# Multithreaded HTTP Server (C++)
A lightweight HTTP/1.1 server built from scratch with **epoll**, **non-blocking sockets**, and a **thread pool**. Designed to showcase systems skills (OS, networking, debugging, performance) for infrastructure roles.


## Build
```bash
mkdir -p build && cd build
cmake .. && cmake --build . -j
```


## Run
```bash
./server 8080 ../www
```
Open http://localhost:8080


## Test
- Functional: `curl -i http://localhost:8080/`
- Concurrency: `ab -n 20000 -c 500 http://127.0.0.1:8080/`
- Alt: `wrk -t4 -c512 -d30s http://127.0.0.1:8080/`


## Features
- HTTP/1.1 parsing (GET)
- Keep-Alive (default on)
- Non-blocking sockets + epoll (edge-triggered)
- Thread pool with work handoff
- Static file serving with sendfile zero-copy for large files
- Small-file LRU memory cache


## Debugging & Profiling
- Memory: `valgrind --leak-check=full ./server 8080 ../www`
- CPU flamegraph (Linux perf): `perf record -g ./server ...` then `perf report`


## Notes
- Linux-only. For Windows, port using Winsock2 + I/O Completion Ports or use standalone Asio.
- Security: basic path traversal protection; for production add stricter checks.


## Resume Bullets (example)
- Built a C++ HTTP server using epoll + thread pool handling 1,000+ concurrent requests; implemented zero-copy `sendfile`, connection reuse (keep-alive), and LRU caching to reduce median latency by ~40% under `ab`.
- Diagnosed race conditions and fixed epoll edge-triggering stalls; used Valgrind to eliminate memory leaks and perf to optimize hot paths.