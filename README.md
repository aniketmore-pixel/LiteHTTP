# LiteHTTP – Multithreaded C++ HTTP Server

![LiteHTTP Banner](https://img.shields.io/badge/LiteHTTP-C++-blue?style=flat-square)

**LiteHTTP** is a lightweight, multithreaded HTTP server written in C++. It serves static files efficiently using an **LRU cache** and a **thread pool**, designed for demonstration, experimentation, and benchmarking. The server uses **non-blocking I/O with epoll** for high concurrency.

---

## Table of Contents

- [Features](#features)  
- [Project Structure](#project-structure)  
- [Requirements](#requirements)  
- [Installation & Running](#installation--running)  
- [Server Usage](#server-usage)  
- [Logging & Monitoring](#logging--monitoring)  
- [File Overview](#file-overview)  
- [Demo](#demo)  
- [Future Enhancements](#future-enhancements)  
- [License](#license)  

---

## Features

-  **Multithreaded** request handling using a configurable thread pool  
-  **LRU cache** for small static files (<256KB) to improve response speed  
-  **Non-blocking I/O** using `epoll` for high concurrency  
-  **HTTP/1.1 compliant** with default keep-alive support  
-  **Thread-safe logging** with timestamps and optional color coding  
-  Lightweight and portable — runs smoothly in **Linux environments**, including Gitpod  
-  Automatic handling of `/` → serves `index.html`  

---

## Project Structure

LiteHTTP/
│
├── www/
│ └── index.html # Sample homepage
│
├── include/
│ ├── http.h # HTTP request/response structures
│ ├── cache.h # LRU cache implementation
│ └── thread_pool.h # Thread pool implementation
│
├── src/
│ ├── http.cpp # HTTP parsing & response functions
│ └── main.cpp # Server main loop and epoll I/O
│
├── Makefile # Build instructions
└── README.md # Project documentation
