cmake_minimum_required(VERSION 3.16)

project(VVCM_CPP VERSION 1.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 包含子目录
include_directories(include)
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(pybind11)
