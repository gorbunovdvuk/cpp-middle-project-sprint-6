set(target queue_test)

find_package(magic_enum REQUIRED)

add_executable(${target}
    bounded_queue.cpp
    unbounded_queue.cpp
    priority_queue.cpp
)

target_link_libraries(${target}
    PRIVATE
        GTest::GTest
        GTest::Main
        queue
        magic_enum::magic_enum
)

add_test(NAME ${target} COMMAND ${target})