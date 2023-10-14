cmake_minimum_required(VERSION 3.25)
project(csv C)

set(CMAKE_C_STANDARD 11)
find_package(PythonLibs 3.6 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -fsanitize=address -fsanitize=undefined")
add_executable(csv serz.c)
target_link_libraries(csv ${PYTHON_LIBRARIES})

