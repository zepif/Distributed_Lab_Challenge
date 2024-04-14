cmake_minimum_required(VERSION 3.10)
project(OrderBook)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(orderbook STATIC
    orderbook.cpp
)

add_executable(orderbook_tests
    tests.cpp
)

target_link_libraries(orderbook_tests
    orderbook
)
