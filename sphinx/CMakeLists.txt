# SPDX-License-Identifier: MIT

add_subdirectory(vendor)

add_executable(sphinx WIN32)

target_compile_features(sphinx PUBLIC cxx_std_23)

set(
		SPHX_SOURCES
		main.cpp
		screen.cpp
		hid.cpp)

set(
		SPHX_HEADERS
		screen.hpp
		hid.hpp)

list(TRANSFORM SPHX_SOURCES PREPEND ${CMAKE_CURRENT_LIST_DIR}/src/)
list(TRANSFORM SPHX_HEADERS PREPEND ${CMAKE_CURRENT_LIST_DIR}/include/sphinx/)

target_sources(sphinx PRIVATE ${SPHX_SOURCES})
target_sources(sphinx PUBLIC ${SPHX_HEADERS})

target_precompile_headers(faroela PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include/sphinx/pch.hpp)

target_include_directories(sphinx PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include/)

target_link_libraries(sphinx PRIVATE faroela glfw)

cxx_default_diagnostics(sphinx)
