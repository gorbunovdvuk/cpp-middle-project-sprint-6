#pragma once
#include "queue/queue.hpp"
#include "types.hpp"

#include <mutex>
#include <queue>
#include <semaphore>

namespace dispatcher::queue {

class BoundedQueue final : public IQueue {
public:
    explicit BoundedQueue(int capacity): size_(0), empty_(capacity) {}

    void push(Task task) override;

    std::optional<Task> try_pop() override;

private:
    std::counting_semaphore<> size_, empty_;
    std::queue<Task> queue_;
    mutable std::mutex mutex_;
};

}  // namespace dispatcher::queue
