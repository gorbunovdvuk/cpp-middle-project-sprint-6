#pragma once

#include <memory>

#include "queue/priority_queue.hpp"
#include "thread_pool/thread_pool.hpp"
#include "types.hpp"

#include <future>

namespace dispatcher {

class TaskDispatcher {
public:
    explicit TaskDispatcher(size_t thread_count, std::map<TaskPriority, queue::QueueOptions> config = {});

    template <typename F, typename... Args>
    auto schedule(TaskPriority priority, F&& function, Args&&... args) {
        using return_type = decltype(std::forward<F>(function)(std::forward<Args>(args)...));

        auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(function), std::forward<Args>(args)...));
        std::future<return_type> result = task->get_future();
        queue_->push(priority, [task]() { (*task)(); });
        return result;
    }

private:
    std::shared_ptr<queue::PriorityQueue> queue_;
    thread_pool::ThreadPool thread_pool_;
};

}  // namespace dispatcher