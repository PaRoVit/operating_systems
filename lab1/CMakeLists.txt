set(CMAKE_CXX_STANDARD 17)


add_executable(lab1 main.cpp src/parent.cpp src/utils.cpp include/utils.hpp)
target_include_directories(lab1 PRIVATE include)

add_executable(child1 src/child1.cpp src/utils.cpp)
target_include_directories(child1 PRIVATE include)

add_executable(child2 src/child2.cpp src/utils.cpp)
target_include_directories(child2 PRIVATE include)


add_dependencies(lab1 child1 child2)