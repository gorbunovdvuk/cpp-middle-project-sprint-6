#pragma once
#include "queue.hpp"
#include "types.hpp"

#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <condition_variable>

namespace dispatcher::queue {

class PriorityQueue {
public:
    explicit PriorityQueue(const std::map<TaskPriority, QueueOptions>& queues);

    void push(TaskPriority priority, Task task);

    std::optional<Task> pop();

    void shutdown();

private:
    std::map<TaskPriority, std::shared_ptr<IQueue>> queues_;
    mutable std::mutex mutex_;
    std::condition_variable not_empty_or_shutdown_;

    bool shutdown_ = false;
};

}  // namespace dispatcher::queue