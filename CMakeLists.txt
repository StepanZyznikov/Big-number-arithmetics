cmake_minimum_required(VERSION 3.27)
project(1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

add_library(bignum STATIC bignum.cpp bignum.h)

add_executable(pi pi.cpp)

target_link_libraries(pi PRIVATE bignum)

# Google tests
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

#For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(tests tests.cpp)
target_link_libraries(tests PRIVATE bignum GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)