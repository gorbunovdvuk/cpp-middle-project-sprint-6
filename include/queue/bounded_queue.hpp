#pragma once
#include "queue/queue.hpp"
#include "types.hpp"

#include <condition_variable>
#include <mutex>
#include <queue>

namespace dispatcher::queue {

class BoundedQueue : public IQueue {
public:
    explicit BoundedQueue(int capacity): max_capacity_(capacity) {}

    void push(Task task) override;

    std::optional<Task> try_pop() override;

private:
    const size_t max_capacity_;
    std::queue<Task> queue_;
    std::condition_variable not_full_;
    mutable std::mutex mutex_;
};

}  // namespace dispatcher::queue
