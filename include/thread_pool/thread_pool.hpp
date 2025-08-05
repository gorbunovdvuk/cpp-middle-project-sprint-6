#pragma once

#include "queue/priority_queue.hpp"

#include <thread>

namespace dispatcher::thread_pool {

class ThreadPool {
public:
    ThreadPool(std::shared_ptr<queue::PriorityQueue> priority_queue, size_t thread_count);

    ~ThreadPool();

private:
    void worker_loop();

    std::shared_ptr<queue::PriorityQueue> priority_queue_;
    std::vector<std::thread> threads_;
};

} // namespace dispatcher::thread_pool
