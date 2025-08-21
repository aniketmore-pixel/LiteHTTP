#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <atomic>


class ThreadPool {
public:
explicit ThreadPool(size_t n) : stop_(false) {
workers_.reserve(n);
for (size_t i = 0; i < n; ++i) {
workers_.emplace_back([this]{
for(;;) {
std::function<void()> task;
{
std::unique_lock<std::mutex> lock(mu_);
cv_.wait(lock, [this]{ return stop_ || !tasks_.empty(); });
if (stop_ && tasks_.empty()) return;
task = std::move(tasks_.front());
tasks_.pop();
}
task();
}
});
}
}


~ThreadPool(){
{
std::lock_guard<std::mutex> lock(mu_);
stop_ = true;
}
cv_.notify_all();
for (auto &t : workers_) t.join();
}


void enqueue(std::function<void()> f){
{
std::lock_guard<std::mutex> lock(mu_);
tasks_.push(std::move(f));
}
cv_.notify_one();
}
private:
std::vector<std::thread> workers_;
std::mutex mu_;
std::condition_variable cv_;
std::queue<std::function<void()>> tasks_;
std::atomic<bool> stop_;
};