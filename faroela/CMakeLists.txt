# SPDX-License-Identifier: MIT

add_subdirectory(vendor)

add_library(faroela STATIC)

target_compile_features(faroela PUBLIC cxx_std_23)

set(
		FARO_SOURCES
		core.cpp
		log.cpp)

set(
		FARO_HEADERS
		core.hpp
		log.hpp)

list(TRANSFORM FARO_SOURCES PREPEND ${CMAKE_CURRENT_LIST_DIR}/src/)
list(TRANSFORM FARO_HEADERS PREPEND ${CMAKE_CURRENT_LIST_DIR}/include/faroela/)

target_sources(faroela PRIVATE ${FARO_SOURCES})
target_sources(faroela PUBLIC ${FARO_HEADERS})

target_precompile_headers(faroela PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include/faroela/pch.hpp)

target_include_directories(faroela PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include/)

set_target_properties(
		faroela PROPERTIES
		CXX_VISIBILITY_PRESET hidden)

target_link_libraries(faroela PRIVATE spdlog Tracy::TracyClient)

cxx_default_diagnostics(faroela)
