#include "queue/priority_queue.hpp"

#include "queue/bounded_queue.hpp"
#include "queue/unbounded_queue.hpp"

#include <ranges>

namespace dispatcher::queue {

PriorityQueue::PriorityQueue(const std::map<TaskPriority, QueueOptions>& queues) {
    for (const auto &[priority, options] : queues) {
        if (options.bounded) {
            queues_.emplace(priority, std::make_shared<BoundedQueue>(options.capacity.value()));
        } else {
            queues_.emplace(priority, std::make_shared<UnboundedQueue>());
        }
    }
}

void PriorityQueue::push(TaskPriority priority, Task task) {
    std::shared_ptr<IQueue> queue = std::invoke(
        [this](TaskPriority priority) {
            std::lock_guard lock(mutex_);
            return queues_.at(priority);
        },
        priority);
    queue->push(std::move(task));
    not_empty_or_shutdown_.notify_one();
}

std::optional<Task> PriorityQueue::pop() {
    std::unique_lock lock(mutex_);

    for (;;) {
        for (auto &queue : queues_ | std::views::values) {
            if (auto task = queue->try_pop(); task.has_value()) {
                return task;
            }
        }

        if (shutdown_) {
            return std::nullopt;
        }

        not_empty_or_shutdown_.wait(lock);
    }
}
void PriorityQueue::shutdown() {
    std::unique_lock lock(mutex_);
    shutdown_ = true;
    not_empty_or_shutdown_.notify_all();
}
} // namespace dispatcher::queue