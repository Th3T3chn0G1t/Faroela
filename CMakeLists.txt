# SPDX-License-Identifier: MIT

# TODO: More of these can be optional -- use less optimal impls in older cmake.
# `add_custom_command(BYPRODUCTS)`: 3.2
# `if(VERSION_GREATER_EQUAL)`: 3.7
# `cmake_policy(CMP0069)`/`check_ipo_supported()`: 3.9
# `string(PREPEND)`: 3.10
# `find_package(Python3)`: 3.12
# `list(JOIN)`: 3.12
# `file(GLOB_RECURSE CONFIGURE_DEPENDS)`: 3.12
# `target_link_options()`: 3.13
# `cmake_policy(CMP0077)`: 3.13
# `target_precompile_headers()`: 3.16
# `CMAKE_CURRENT_FUNCTION_LIST_DIR`: 3.17
# `cmake_language()`: 3.18
# `target_compile_features(cxx_std_23)`: 3.20
# `cmake_policy(CMP0118)`: 3.20
# `CMAKE_GENERATOR "Visual Studio 17 2022"`: 3.21
# `$<LINK_LIBRARY:WHOLE_ARCHIVE>`: 3.24
# <optional> `Python3_INTERPRETER`: 3.30
# <optional> `cmake_policy(CMP0163)`: 3.30
# <optional> `add_custom_command(CODEGEN)`: 3.31
cmake_minimum_required(VERSION 3.24...3.31 FATAL_ERROR)

if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.31")
	# Enable `add_custom_command(CODEGEN)` enhancements.
	cmake_policy(SET CMP0171 NEW)
endif()

if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.30")
	# The GENERATED source file property is now visible in all directories.
	cmake_policy(SET CMP0163 NEW)
endif()

# Allow `GENERATED` source property to propagate between dirs.
cmake_policy(SET CMP0118 NEW)

# Enforce IPO when set.
cmake_policy(SET CMP0069 NEW)

# Allow variables to propagate to sub-projects.
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
cmake_policy(SET CMP0077 NEW)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(BUILD_SHARED_LIBS OFF)

include(cmake/utilities.cmake)

project(Faroela CXX)

# Enable IPO globally if supported.
include(CheckIPOSupported)
check_ipo_supported(RESULT IPO_SUPPORTED)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ${IPO_SUPPORTED})

# `argparse`: 3.2
find_package(Python3 3.2 REQUIRED COMPONENTS Interpreter)

if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.30")
	set(FAROELA_PYTHON_INTERPRETER ${Python3_INTERPRETER})
else()
	set(FAROELA_PYTHON_INTERPRETER ${Python3_EXECUTABLE})
endif()

add_subdirectory(faroela)
add_subdirectory(pharaoh)
add_subdirectory(sphinx)
