cmake_minimum_required(VERSION 3.25)
project(contest8)

find_package(CURL REQUIRED)
add_executable (contest8 1.c)
include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(contest8  ${CURL_LIBRARIES})
