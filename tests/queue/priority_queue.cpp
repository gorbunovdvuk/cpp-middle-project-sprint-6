#include <gtest/gtest.h>
#include <magic_enum/magic_enum.hpp>

#include "queue/priority_queue.hpp"

using namespace dispatcher;
using namespace dispatcher::queue;

TEST(PriorityQueueTest, SequenceTest) {
    std::map<TaskPriority, QueueOptions> options = {
        {TaskPriority::Low, {.bounded = false}},
        {TaskPriority::Normal, {.bounded = false}},
        {TaskPriority::High, {.bounded = true, .capacity = 5}}
    };
    std::stringstream ss;
    PriorityQueue pq(options);
    for (size_t i = 0; i < 10; i++) {
        pq.push(magic_enum::enum_value<TaskPriority>(i % magic_enum::enum_count<TaskPriority>()), [&ss, i]() { ss << i; });
    }
    for (size_t i = 0; i < 10; i++) {
        pq.pop().value()();
    }
    EXPECT_EQ(ss.str(), "0369147258");
    pq.shutdown();
    EXPECT_FALSE(pq.pop().has_value());
}
