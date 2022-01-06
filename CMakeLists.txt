cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(obfstr CXX)

option(BUILD_SAMPLES "Build ObfStr Sample Programs" ON)

add_library(obfstr INTERFACE)
target_include_directories(obfstr INTERFACE include)

if (BUILD_SAMPLES)
    add_subdirectory(samples)
endif()
