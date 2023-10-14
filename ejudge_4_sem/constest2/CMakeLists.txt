cmake_minimum_required(VERSION 3.24)
project(constest2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=undefined -Wall -Wextra -Werror")
add_executable(constest2 new_task.cpp)
