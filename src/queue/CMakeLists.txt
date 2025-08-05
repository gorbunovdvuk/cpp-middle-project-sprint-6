find_package(Threads REQUIRED)
find_package(magic_enum REQUIRED)

add_library(queue
    bounded_queue.cpp
    unbounded_queue.cpp
    priority_queue.cpp
)

target_link_libraries(queue PUBLIC Threads::Threads magic_enum::magic_enum)
