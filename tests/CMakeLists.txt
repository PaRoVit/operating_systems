include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.11.0
)

FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)


set(CMAKE_CXX_STANDARD 17)


add_executable(kp_test test.cpp ../KP/include/list_allocator.hpp ../KP/src/list_allocator.cpp ../KP/src/buddy_allocator.cpp ../KP/include/buddy_allocator.hpp)

target_link_libraries(kp_test PRIVATE GTest::GTest)
target_include_directories(kp_test PRIVATE ../KP/include)

add_test(kp_test kp_test)