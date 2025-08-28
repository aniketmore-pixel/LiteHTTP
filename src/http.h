#pragma once
#include <string>
#include <unordered_map>

struct HttpRequest {
    std::string method;
    std::string path;
    std::unordered_map<std::string, std::string> headers;
    bool keep_alive{false};
};

struct HttpResponse {
    int status{200};
    std::string status_text{"OK"};
    std::unordered_map<std::string, std::string> headers;
    std::string body;
};

bool parse_http_request(const std::string &raw, HttpRequest &req);
std::string build_http_response_header(const HttpResponse &res);
