cmake_minimum_required(VERSION 3.22)
project(CAPICXX_DETAILED_EXAMPLE)

find_package(CommonAPI REQUIRED)
find_package(CommonAPI-SomeIP REQUIRED)

add_subdirectory(client-proxy)
add_subdirectory(service-stub)
