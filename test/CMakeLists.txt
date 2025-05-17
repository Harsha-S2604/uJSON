cmake_minimum_required(VERSION 3.31.6)

project(test)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

find_package(ujson REQUIRED)


add_executable(main main.c)
target_link_libraries(main PRIVATE ujson::ujson)
