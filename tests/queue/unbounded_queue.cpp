#include <gtest/gtest.h>

#include "queue/unbounded_queue.hpp"

using namespace dispatcher::queue;

TEST(UnboundedQueueTest, SimpleTest) {
    UnboundedQueue queue;
    queue.push([](){});
    queue.push([](){});
    EXPECT_TRUE(queue.try_pop());
    EXPECT_TRUE(queue.try_pop());
    EXPECT_FALSE(queue.try_pop());
}

TEST(UnboundedQueueTest, SequenceTest) {
    UnboundedQueue queue;
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
