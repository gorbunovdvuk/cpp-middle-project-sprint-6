#pragma once
#include "queue/queue.hpp"

#include <condition_variable>
#include <mutex>
#include <queue>
#include <semaphore>

namespace dispatcher::queue {

class UnboundedQueue final : public IQueue {
public:
    UnboundedQueue() = default;

    void push(Task task) override;

    std::optional<Task> try_pop() override;

private:
    std::queue<Task> queue_;
    std::counting_semaphore<> size_{0};
    mutable std::mutex mutex_;
};

}  // namespace dispatcher::queue