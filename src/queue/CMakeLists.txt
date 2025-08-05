find_package(Threads REQUIRED)

add_library(queue
    bounded_queue.cpp
    unbounded_queue.cpp
    priority_queue.cpp
)

target_link_libraries(queue PUBLIC Threads::Threads)
