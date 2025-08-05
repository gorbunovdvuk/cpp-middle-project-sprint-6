#include "thread_pool/thread_pool.hpp"

#include "logger.hpp"

#include <format>

namespace dispatcher::thread_pool {

ThreadPool::ThreadPool(std::shared_ptr<queue::PriorityQueue> priority_queue, size_t thread_count)
    : priority_queue_(std::move(priority_queue)) {
    threads_.reserve(thread_count);
    for (size_t i = 0; i < thread_count; ++i) {
        threads_.emplace_back([this]() { worker_loop(); });
    }
}

ThreadPool::~ThreadPool() {
    priority_queue_->shutdown();
    for (auto &thread : threads_) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void ThreadPool::worker_loop() {
    for (;;) {
        auto task = priority_queue_->pop();
        if (task == std::nullopt) {
            break;
        }
        try {
            std::invoke(*task);
        } catch (std::exception &e) {
            Logger::Get().Log(std::format("Exception occured in thread: {}", e.what()));
        } catch (...) {
            Logger::Get().Log(std::format("Unknown exception occured in thread"));
        }
    }
}

} // namespace dispatcher::thread_pool