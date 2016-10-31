cmake_minimum_required(VERSION 3.5)
project(nichide)

set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu99")

set(SOURCE_FILES main.c)
add_executable(nichide ${SOURCE_FILES})