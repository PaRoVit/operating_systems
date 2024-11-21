set(CMAKE_CXX_STANDARD 17)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/lab4/include)

add_library(lib1 SHARED src/implementation1.cpp)
add_library(lib2 SHARED src/implementation2.cpp)

target_include_directories(lib1 PUBLIC ${INCLUDE_DIR})
target_include_directories(lib2 PUBLIC ${INCLUDE_DIR})

add_executable(static_main static_main.cpp include/implementations.hpp)
add_executable(dynamic_main dynamic_main.cpp src/utils.cpp include/utils.hpp)

target_link_libraries(static_main PRIVATE lib1)

target_include_directories(dynamic_main PRIVATE ${INCLUDE_DIR})