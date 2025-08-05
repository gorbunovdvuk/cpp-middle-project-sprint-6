#include "queue/bounded_queue.hpp"

namespace dispatcher::queue {

void BoundedQueue::push(Task task) {
    std::unique_lock lock(mutex_);
    not_full_.wait(lock, [this]() { return queue_.size() < max_capacity_; });
    queue_.push(task);
}

std::optional<Task> BoundedQueue::try_pop() {
    std::unique_lock lock(mutex_);
    if (queue_.empty()) {
        return std::nullopt;
    }
    Task task = queue_.front();
    queue_.pop();
    lock.unlock();
    not_full_.notify_one();
    return task;
}

} // namespace dispatcher::queue