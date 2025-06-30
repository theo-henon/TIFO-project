cmake_minimum_required(VERSION 3.20)

project(tifo-project VERSION 1.0.0 LANGUAGES CXX)

# Required packages
find_package(TIFF REQUIRED)

include_directories(src)

add_subdirectory(src)