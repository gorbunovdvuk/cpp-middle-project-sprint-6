#include "queue/priority_queue.hpp"

#include "../../../../.conan2/p/magic2b79a91bc8c6c/p/include/magic_enum/magic_enum.hpp"
#include "queue/bounded_queue.hpp"
#include "queue/unbounded_queue.hpp"

#include <ranges>

namespace dispatcher::queue {

void PriorityQueue::push(TaskPriority priority, Task task) {
    std::shared_ptr<IQueue> queue = queues_.at(priority);
    queue->push(std::move(task));
    not_empty_or_shutdown_.notify_one();
}

std::optional<Task> PriorityQueue::pop() {
    for (;;) {
        for (auto &queue : queues_ | std::views::values) {
            if (auto task = queue->try_pop(); task.has_value()) {
                return task;
            }
        }

        std::unique_lock lock(mutex_);
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

std::map<TaskPriority, std::shared_ptr<IQueue>> PriorityQueue::prepare_queues(const std::map<TaskPriority, QueueOptions>& queues) {
    std::map<TaskPriority, std::shared_ptr<IQueue>> queues_;
    for (const auto priority : magic_enum::enum_values<TaskPriority>()) {
        if (const auto [bounded, capacity] = queues.at(priority); bounded) {
            queues_.emplace(priority, std::make_shared<BoundedQueue>(capacity.value()));
        } else {
            queues_.emplace(priority, std::make_shared<UnboundedQueue>());
        }
    }
    return queues_;
}

} // namespace dispatcher::queue
