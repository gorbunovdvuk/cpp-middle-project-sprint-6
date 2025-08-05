#include <gtest/gtest.h>
#include <sstream>
#include <chrono>
#include <future>
#include <thread>

#include "queue/bounded_queue.hpp"

using namespace std::chrono_literals;
using namespace dispatcher::queue;

TEST(BoundedQueueTest, SimpleTest) {
    BoundedQueue queue(3);
    queue.push([](){});
    queue.push([](){});
    EXPECT_TRUE(queue.try_pop());
    EXPECT_TRUE(queue.try_pop());
    EXPECT_FALSE(queue.try_pop());
}

TEST(BoundedQueueTest, SequenceTest) {
    BoundedQueue queue(100);
    std::stringstream ss;
    for (size_t i = 0; i < 10; i++) {
        queue.push([&ss, i]() { ss << i; });
    }
    for (size_t i = 0; i < 10; i++) {
        auto task = queue.try_pop();
        EXPECT_TRUE(task.has_value());
        (*task)();
    }
    EXPECT_EQ(ss.str(), "0123456789");
    EXPECT_FALSE(queue.try_pop().has_value());
}

TEST(BoundedQueueTest, BoundTest) {
    auto queue = std::make_shared<BoundedQueue>(10);
    for (size_t i = 0; i < 10; i++) {
        queue->push([](){});
    }
    std::atomic<bool> finished = false;
    std::thread thread([queue, &finished] {
        queue->push([](){});
        finished = true;
    });
    thread.detach();
    std::this_thread::sleep_for(100ms);
    EXPECT_FALSE(finished.load());
}
