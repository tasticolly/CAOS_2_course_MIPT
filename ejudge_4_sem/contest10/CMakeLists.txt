cmake_minimum_required(VERSION 3.25)
project(contest10)
set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(FUSE REQUIRED fuse3)


link_libraries(${FUSE_LIBRARIES})         # -lfuse3 -lpthread
include_directories(${FUSE_INCLUDE_DIRS}) # -I/usr/include/fuse3
add_executable(contest10 serzio.cpp)