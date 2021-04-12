cmake_minimum_required(VERSION 3.0.0)
project(mobi-metadata VERSION 0.1.0)

##SET(CMAKE_C_COMPILER "clang")
##SET(CMAKE_CXX_COMPILER "clang++")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra  -Wpedantic")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -g --std=c++20")
##set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/mingw64/include")
##set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I/mingw64/include")


SET(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

add_subdirectory(src)
add_subdirectory(tests)
