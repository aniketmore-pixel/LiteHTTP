#include "http.h"
#include <sstream>
#include <algorithm>


static std::string trim(const std::string &s){
size_t a = s.find_first_not_of(" \r\n\t");
if (a == std::string::npos) return "";
size_t b = s.find_last_not_of(" \r\n\t");
return s.substr(a, b - a + 1);
}


bool parse_http_request(const std::string &raw, HttpRequest &req){
std::istringstream ss(raw);
std::string line;
if (!std::getline(ss, line)) return false;
if (!line.empty() && line.back()=='\r') line.pop_back();
std::istringstream start(line);
if (!(start >> req.method >> req.path)) return false;


// headers
while (std::getline(ss, line)){
if (!line.empty() && line.back()=='\r') line.pop_back();
if (line.empty()) break;
auto pos = line.find(":");
if (pos==std::string::npos) continue;
std::string k = trim(line.substr(0,pos));
std::string v = trim(line.substr(pos+1));
std::transform(k.begin(), k.end(), k.begin(), ::tolower);
req.headers[k]=v;
}
auto it = req.headers.find("connection");
if (it!=req.headers.end()){
std::string v = it->second; std::transform(v.begin(), v.end(), v.begin(), ::tolower);
req.keep_alive = (v.find("keep-alive")!=std::string::npos);
} else {
// HTTP/1.1 default keep-alive
req.keep_alive = true;
}
return true;
}


std::string build_http_response_header(const HttpResponse &res){
std::ostringstream out;
out << "HTTP/1.1 " << res.status << ' ' << res.status_text << "\r\n";
for (auto &kv : res.headers){
out << kv.first << ": " << kv.second << "\r\n";
}
out << "\r\n";
return out.str();
}