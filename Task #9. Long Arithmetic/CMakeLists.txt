cmake_minimum_required(VERSION 3.10)
project(BigInt)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(BigInt STATIC
  BigInt.cpp
  BigInt.h
)

add_executable(BigIntApp
  main.cpp
)

target_link_libraries(BigIntApp PUBLIC BigInt)
