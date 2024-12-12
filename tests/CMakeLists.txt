include(FetchContent)
cmake_minimum_required(VERSION 3.5)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.11.0
)

FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-lzmq")
add_compile_options(-Wno-error=deprecated-declarations -Wdeprecated-declarations)

add_executable(lab567_test test.cpp ../lab5-7/src/zmq_manage.cpp ../lab5-7/src/tree.cpp)
target_link_libraries(lab567_test PRIVATE GTest::GTest)
target_include_directories(lab567_test PRIVATE ../lab5-7/include)

target_link_libraries(lab567_test PRIVATE zmq)
