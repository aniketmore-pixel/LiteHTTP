LiteHTTP – Multithreaded C++ HTTP Server

LiteHTTP is a lightweight, multithreaded HTTP server written in C++. It serves static files efficiently using an LRU cache and a thread pool, designed for demonstration and benchmarking in educational and experimental environments.

Table of Contents

Features

Project Structure

Requirements

Installation & Running

Usage

Logging & Monitoring

Demo Screenshots

Future Enhancements

License

Features

✅ Multithreaded request handling using a configurable thread pool.

✅ LRU caching for small static files (<256KB) to improve response speed.

✅ Non-blocking I/O using epoll for high concurrency.

✅ HTTP/1.1 compliant with keep-alive support.

✅ Thread-safe logging with timestamps and optional color-coding for demonstration.

✅ Lightweight and portable — runs smoothly in Linux environments, including Gitpod.

Project Structure
LiteHTTP/
│
├── www/
│   └── index.html        # Sample homepage
│
├── include/
│   ├── http.h            # HTTP request/response structures
│   ├── cache.h           # LRU cache implementation
│   └── thread_pool.h     # Thread pool implementation
│
├── src/
│   ├── http.cpp          # HTTP parsing & response functions
│   └── main.cpp          # Server main loop and epoll I/O
│
├── Makefile              # Build instructions
└── README.md             # Project documentation

Requirements

C++17 or higher

Linux or Unix-based environment (Gitpod, Ubuntu, etc.)

Standard build tools: g++, make

Network access for port binding

Installation & Running

Clone the repository:

git clone https://github.com/yourusername/LiteHTTP.git
cd LiteHTTP


Build the project using Makefile:

make


Run the server:

./server 8080 www


8080 → Port number (optional, default 8080)

www → Document root directory (optional, default www)

Open in browser:

Gitpod automatically exposes port 8080. Click Ports → Open Browser.

URL format: https://8080-<workspace-id>.gitpod.io

Usage

Navigate to the server URL in your browser to see index.html.

Refresh or request other files in www/ to test caching and multithreaded handling.

Logs appear in real-time in the terminal:

[15:30:10] ✅ Accepted new client: fd=6
[15:30:10] 📄 Request received: GET /index.html
[15:30:10] ✅ Response sent to fd=6 (status: 200)


Supports simultaneous connections using the ThreadPool.

Logging & Monitoring

Terminal logs: Timestamped, color-coded messages for:

Server startup/shutdown

Client connections

Requests and responses

Errors and exceptions

Optional file logging: Writes to server.log for persistent record.

Demo Screenshots

✅ Server startup and accepted client logs:

[15:23:42] 🚀 Starting LiteHTTP on port 8080
[15:23:45] ✅ Accepted new client: fd=5
[15:23:46] 📄 Request received: GET /index.html
[15:23:46] ✅ Response sent to fd=5 (status: 200)


✅ Browser displaying index.html:

It works 🎉
Multithreaded HTTP server (C++)

Future Enhancements

Support for dynamic content and APIs

HTTPS support with TLS/SSL

Improved MIME type handling

Configurable logging verbosity

Detailed benchmarking tools

License

This project is open-source under the MIT License.
Feel free to modify, reuse, and distribute.

💡 Tip: For Gitpod demos, use terminal + browser side by side to show live requests, logs, and dynamic page updates simultaneously.
