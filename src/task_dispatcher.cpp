#include "task_dispatcher.hpp"

namespace dispatcher {

namespace {

std::map<TaskPriority, queue::QueueOptions> get_default_config() {
    return {{TaskPriority::Low, {.bounded = false}},
            {TaskPriority::Normal, {.bounded = false}},
            {TaskPriority::High, {.bounded = true, .capacity = 10}}};
}

}  // namespace

TaskDispatcher::TaskDispatcher(size_t thread_count, std::map<TaskPriority, queue::QueueOptions> config)
    : queue_(std::make_shared<queue::PriorityQueue>(config.empty() ? get_default_config() : std::move(config))),
      thread_pool_(queue_, thread_count) {}

} // namespace dispatcher