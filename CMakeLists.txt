# TODO: Make these optional -- use less optimal impls in older cmake.
# `cmake_policy(CMP0069)`/`check_ipo_supported()`: 3.9
# `string(PREPEND)`: 3.10
# `find_package(Python3)`: 3.12
# `list(JOIN)`: 3.12
# `file(GLOB_RECURSE CONFIGURE_DEPENDS)`: 3.12
# `target_link_options()`: 3.13
# `cmake_policy(CMP0077)`: 3.13
# `target_precompile_headers()`: 3.16
# `cmake_language()`: 3.18
# `target_compile_features(cxx_std_23)`: 3.20
cmake_minimum_required(VERSION 3.20)

# Enforce IPO when set.
set(CMAKE_POLICY_DEFAULT_CMP0069 NEW)
cmake_policy(SET CMP0069 NEW)

# Allow variables to propagate to sub-projects.
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
cmake_policy(SET CMP0077 NEW)

include(cmake/Utilities.cmake)

project(FARO CXX)

option(FARO_ENABLE_ASAN "Enable building with address sanitizer if available" OFF)
option(FARO_ENABLE_UBSAN "Enable building with undefined behaviour sanitizer if available" OFF)
option(FARO_ENABLE_MSAN "Enable building with memory sanitizer if available" OFF)
option(FARO_ENABLE_TSAN "Enable building with thread sanitizer if available" OFF)
option(FARO_ENABLE_CFISAN "Enable building with control flow integrity sanitizer if available" OFF)

# Enable IPO globally if supported.
include(CheckIPOSupported)
check_ipo_supported(RESULT IPO_SUPPORTED)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ${IPO_SUPPORTED})

add_subdirectory(vendor)
