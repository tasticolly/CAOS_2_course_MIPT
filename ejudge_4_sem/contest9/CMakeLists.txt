cmake_minimum_required(VERSION 3.25)
project(contest8)

find_package(OpenSSL COMPONENTS Crypto REQUIRED)
add_executable (contest8 1.c)
include_directories(${OPENSSL_INCLUDE_DIR})
target_link_libraries(contest8  ${OPENSSL_CRYPTO_LIBRARY})
