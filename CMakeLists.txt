cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_definitions(-D_WIN32_WINNT=0x0601)

project ("rewin" C CXX)

add_subdirectory(rewin-utility)
add_subdirectory(rewin)
add_subdirectory(rewin-example)