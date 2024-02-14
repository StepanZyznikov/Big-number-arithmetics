cmake_minimum_required(VERSION 3.27)
project(1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast")

add_executable(1 main.cpp
        bignum.h
        bignum.cpp
        pi.cpp
)