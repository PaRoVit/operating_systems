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


add_executable(lab4_test lab4_test.cpp ../lab4/include/implementations.hpp ../lab4/include/utils.hpp ../lab4/src/implementation1.cpp ../lab4/src/implementation2.cpp ../lab4/src/utils.cpp)

target_link_libraries(lab4_test PRIVATE GTest::GTest)
target_include_directories(lab4_test PRIVATE ../lab4/include)

add_test(lab4_test lab4_test)