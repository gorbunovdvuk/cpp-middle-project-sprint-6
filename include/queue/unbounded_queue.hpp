#pragma once
#include "queue/queue.hpp"

#include <condition_variable>
#include <mutex>
#include <queue>

namespace dispatcher::queue {

class UnboundedQueue : public IQueue {
public:
    UnboundedQueue() = default;

    void push(Task task) override;

    std::optional<Task> try_pop() override;

private:
    std::queue<Task> queue_;
    mutable std::mutex mutex_;
};

}  // namespace dispatcher::queue