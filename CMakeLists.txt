cmake_minimum_required(VERSION 3.16)

project(VVCM VERSION 0.0.1 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include subdirectories
include_directories(include)
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(python_bindings)
