# SPDX-License-Identifier: MIT

# TODO: This is copied out of the actual `eternal` CMakeLists.txt because
#       `SOURCE_DIR` does not behave properly as a subproject as-used.

cmake_minimum_required(VERSION 3.6)

project(eternal LANGUAGES CXX)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(eternal INTERFACE)
target_include_directories(eternal SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../../eternal/include)
