set(CMAKE_CXX_STANDARD 17)


add_executable(kp main.cpp src/list_allocator.cpp src/buddy_allocator.cpp)
target_include_directories(kp PRIVATE include)
