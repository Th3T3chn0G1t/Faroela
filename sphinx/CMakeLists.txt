# SPDX-License-Identifier: MIT

add_subdirectory(vendor)

add_executable(sphinx WIN32)

target_compile_features(sphinx PUBLIC cxx_std_23)

set(
		SPHINX_SOURCES
		main.cpp
		screen.cpp)

set(
		SPHINX_HEADERS
		screen.hpp)

list(TRANSFORM SPHINX_SOURCES PREPEND ${CMAKE_CURRENT_LIST_DIR}/src/)
list(TRANSFORM SPHINX_HEADERS PREPEND ${CMAKE_CURRENT_LIST_DIR}/include/sphinx/)

target_sources(sphinx PRIVATE ${SPHINX_SOURCES})
target_sources(sphinx PUBLIC ${SPHINX_HEADERS})

target_precompile_headers(sphinx PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include/sphinx/pch.hpp)

target_include_directories(sphinx PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include/)

target_link_libraries(sphinx PRIVATE faroela glfw)

cxx_default_diagnostics(sphinx)
