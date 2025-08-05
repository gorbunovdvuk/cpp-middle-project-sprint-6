#include <gtest/gtest.h>

#include "task_dispatcher.hpp"

#include <chrono>

using namespace dispatcher;
using namespace std::chrono_literals;

TEST(TaskDispatcherTest, SimpleTest) {
    TaskDispatcher td(1);
    auto result = td.schedule(TaskPriority::High, [](int a, int b) { return a * b; }, 3, 2);
    EXPECT_EQ(result.get(), 6);
}

TEST(TaskDispatcherTest, TestDestructor) {
    const auto thread_count = std::thread::hardware_concurrency();

    std::atomic<size_t> counter = 0;
    {
        TaskDispatcher td(thread_count);
        for (size_t i = 0; i < thread_count * 2; i++) {
            td.schedule(TaskPriority::Low, [&counter]() { std::this_thread::sleep_for(100ms); ++counter; });
        }
    }
    EXPECT_EQ(counter, thread_count * 2);
}

TEST(TaskDispatcherTest, TestExceptions) {
    TaskDispatcher td(1);
    auto result = td.schedule(TaskPriority::High, []() { throw std::runtime_error("hello"); });
    EXPECT_THROW(result.get(), std::runtime_error);
}
