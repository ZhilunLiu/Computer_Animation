#=============================================================================#
# Project setup.
#=============================================================================#
project(atlas-examples)

cmake_minimum_required(VERSION 3.0)

#=============================================================================#
# Tell compiler to use C++11.
#=============================================================================#
SET(CMAKE_CXX_FLAGS "-std=c++11 -O3")

#=============================================================================#
# Directory Variables.
#=============================================================================#
set(EX_SOURCE_DIR "${PROJECT_SOURCE_DIR}")
set(EX_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}")
set(EX_ATLAS_ROOT "${EX_SOURCE_DIR}/lib/atlas")
set(EX_DATA_ROOT "${EX_SOURCE_DIR}/data")
set(EX_ROOT "${EX_SOURCE_DIR}/examples")

#=============================================================================#
# Subdirectories.
#=============================================================================#
add_subdirectory(${EX_ATLAS_ROOT})
include_directories("${ATLAS_INCLUDE_DIRS}")

add_subdirectory(${EX_DATA_ROOT})
add_subdirectory(${EX_ROOT})
