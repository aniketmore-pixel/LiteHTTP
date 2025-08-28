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
<img width="233" height="223" alt="image" src="https://github.com/user-attachments/assets/d7bf219a-0def-4d2b-8999-ee4991cba0c9" />


---

## Requirements

- C++17 or higher  
- Linux or Unix-based OS (Ubuntu, Gitpod, etc.)  
- Build tools: `g++`, `make`  
- Network access for port binding  

---

## Installation & Running

1. Clone the repository:

```bash
git clone https://github.com/yourusername/LiteHTTP.git
cd LiteHTTP
```

2. Build the project:
```bash
make
```

3. Run the server:
```bash
./server 8080 www
```

4. Open in browser (Gitpod):
```bash
   The URL format will be:  https://8080-<workspace-id>.gitpod.io
```
