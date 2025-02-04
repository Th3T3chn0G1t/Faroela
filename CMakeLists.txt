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
# `CMAKE_GENERATOR "Visual Studio 17 2022"`: 3.21
cmake_minimum_required(VERSION 3.21)

# Enforce IPO when set.
set(CMAKE_POLICY_DEFAULT_CMP0069 NEW)
cmake_policy(SET CMP0069 NEW)

# Allow variables to propagate to sub-projects.
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
cmake_policy(SET CMP0077 NEW)

include(cmake/Utilities.cmake)

project(FARO CXX)

# Enable IPO globally if supported.
include(CheckIPOSupported)
check_ipo_supported(RESULT IPO_SUPPORTED)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ${IPO_SUPPORTED})

add_subdirectory(faroela)

include(ExternalProject)

set(PHARO_BINDIR ${CMAKE_CURRENT_BINARY_DIR}/pharaoh/)

ExternalProject_Add(
	pharaoh
	CONFIGURE_COMMAND ""
	INSTALL_COMMAND ""
	SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/pharaoh
	BUILD_COMMAND
		dotnet build ${CMAKE_CURRENT_LIST_DIR}/pharaoh/pharaoh.csproj
		--artifacts-path ${CMAKE_CURRENT_BINARY_DIR}/pharaoh
		--output ${PHARO_BINDIR}
		--self-contained $<IF:$<CONFIG:Debug>,true,false>)

add_custom_command(
	TARGET pharaoh POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		$<TARGET_FILE:faroela> ${PHARO_BINDIR})
