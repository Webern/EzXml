cmake_minimum_required(VERSION 3.12)
project(ezxml)

set(CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE SRC_PRIVATE ./src/*.c* ./src/*.h*)
file(GLOB_RECURSE SRC_PUBLIC ./include/ezxml/*.h)

add_library(ezxml STATIC ${SRC_PRIVATE} ${SRC_PUBLIC})
source_group( "private" FILES ${SRC_PRIVATE} )
source_group( "public" FILES ${SRC_PUBLIC} )

target_include_directories(ezxml PUBLIC include)
target_include_directories(ezxml PRIVATE .)
