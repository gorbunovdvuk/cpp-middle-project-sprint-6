#include "queue/unbounded_queue.hpp"

#include <functional>
#include <mutex>
#include <queue>
#include <semaphore>

namespace dispatcher::queue {

void UnboundedQueue::push(Task task) {
    std::unique_lock lock(mutex_);
    queue_.push(task);
}

std::optional<Task> UnboundedQueue::try_pop() {
    std::unique_lock lock(mutex_);
    if (queue_.empty()) {
        return std::nullopt;
    }
    Task task = queue_.front();
    queue_.pop();
    return task;
}

} // namespace dispatcher::queue