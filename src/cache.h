#pragma once
#include <unordered_map>
#include <list>
#include <string>
#include <utility>


// Simple LRU cache for small static files (<256KB)
class LRUCache {
public:
explicit LRUCache(size_t cap_bytes): cap_(cap_bytes), used_(0) {}


bool get(const std::string &key, std::string &val){
auto it = map_.find(key);
if (it==map_.end()) return false;
order_.splice(order_.begin(), order_, it->second.second);
val = it->second.first; return true;
}


void put(const std::string &key, std::string val){
size_t sz = val.size();
if (sz > cap_) return;
if (auto it = map_.find(key); it!=map_.end()){
used_ -= it->second.first.size();
order_.erase(it->second.second);
map_.erase(it);
}
while (used_ + sz > cap_ && !order_.empty()){
auto last = order_.back(); order_.pop_back();
used_ -= map_[last].first.size();
map_.erase(last);
}
order_.push_front(key);
map_[key] = {std::move(val), order_.begin()};
used_ += sz;
}
private:
size_t cap_, used_;
std::list<std::string> order_;
std::unordered_map<std::string, std::pair<std::string, std::list<std::string>::iterator>> map_;
};