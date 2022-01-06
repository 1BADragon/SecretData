cmake_minimum_required(VERSION 3.10)
project(obfstr CXX)

option(BUILD_SAMPLES "Build ObfStr Sample Programs" ON)

add_library(obfstr INTERFACE)
target_include_directories(obfstr INTERFACE include)

if (BUILD_SAMPLES)
    add_subdirectory(samples)
endif()
