#include "queue/unbounded_queue.hpp"

#include <functional>
#include <mutex>
#include <queue>

namespace dispatcher::queue {

void UnboundedQueue::push(Task task) {
    std::lock_guard lock(mutex_);
    queue_.push(task);
    size_.release();
}

std::optional<Task> UnboundedQueue::try_pop() {
    if (!size_.try_acquire()) {
        return std::nullopt;
    }
    std::unique_lock lock(mutex_);
    Task task = std::move(queue_.front());
    queue_.pop();
    return task;
}

} // namespace dispatcher::queue
