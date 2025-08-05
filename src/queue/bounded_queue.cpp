#include "queue/bounded_queue.hpp"

namespace dispatcher::queue {

void BoundedQueue::push(Task task) {
    empty_.acquire();
    std::lock_guard lock(mutex_);
    queue_.push(task);
    size_.release();
}

std::optional<Task> BoundedQueue::try_pop() {
    if (!size_.try_acquire()) {
        return std::nullopt;
    }
    std::unique_lock lock(mutex_);
    Task task = std::move(queue_.front());
    queue_.pop();
    lock.unlock();
    empty_.release();
    return task;
}

} // namespace dispatcher::queue
