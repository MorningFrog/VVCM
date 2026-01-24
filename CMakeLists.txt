cmake_minimum_required(VERSION 3.16)

# Read version from file `VERSION`
file(STRINGS "${CMAKE_SOURCE_DIR}/VERSION" PROJECT_VERSION)
string(STRIP "${PROJECT_VERSION}" PROJECT_VERSION)

project(VVCM VERSION ${PROJECT_VERSION} LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Fetch Eigen3
include(FetchContent)

FetchContent_Declare(
  eigen
  URL https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.tar.gz
)
FetchContent_MakeAvailable(eigen)

# Include subdirectories
include_directories(include)
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(python_bindings)
