cmake_minimum_required(VERSION 3.20)

project(tifo-project VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Required packages
find_package(TIFF REQUIRED)

include_directories(src)

add_subdirectory(src)