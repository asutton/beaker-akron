# Copyright (c) 2015-2016 Andrew Sutton
# All rights reserved


# Project configuration
# Add ~/local/include to the include search path.
cmake_minimum_required(VERSION 3.1)
set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} $ENV{HOME}/local/include)

# Define the project.
project(beaker VERSION 0.0 LANGUAGES CXX)
enable_testing()

set(CMAKE_CXX_FLAGS "-Wall -std=c++11")

# Determine architecture.
math(EXPR bits "8 * ${CMAKE_SIZEOF_VOID_P}")
add_definitions(-DBEAKER_ARCH=${bits})


# libfmt dependencies
find_path(FMT_INCLUDE_DIR fmt/format.h)
find_library(FMT_LIBRARY_DIR fmt)
message(STATUS "libfmt include dir: ${FMT_INCLUDE_DIR}")
message(STATUS "libfmt library: ${FMT_LIBRARY_DIR}")
include_directories(${FMT_INCLUDE_DIR})


# RapidJSON dependencies.
find_path(JSON_INCLUDE_DIR rapidjson/fwd.h)
message(STATUS "RapidJSON include dir: ${JSON_INCLUDE_DIR}")
include_directories(${JSON_INCLUDE_DIR})


# Boost dependencies
find_package(Boost 1.55.0 REQUIRED COMPONENTS system filesystem program_options)


# LLVM dependencies
find_package(LLVM REQUIRED CONFIG)
llvm_map_components_to_libnames(LLVM_LIBRARIES core)
message(STATUS "LLVM include dir: ${LLVM_INCLUDE_DIRS}")
add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})


# Compiler configuration
include_directories(.)


add_subdirectory(beaker)
# add_subdirectory(tests)

