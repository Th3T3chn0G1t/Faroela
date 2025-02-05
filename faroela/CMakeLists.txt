# SPDX-License-Identifier: MIT

add_subdirectory(vendor)

add_library(faroela MODULE)

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
		CXX_VISIBILITY_PRESET hidden
		PREFIX "")

target_link_libraries(faroela PRIVATE spdlog Tracy::TracyClient)

cxx_options_conditional(
		COMMAND target_compile_options
		ARGS faroela PRIVATE
		FLAGS -Wall -Wextra -Werror -pedantic -pedantic-errors)

cxx_options_conditional(
		COMMAND target_compile_options
		ARGS faroela PRIVATE
		FLAGS /Wall /WX
			/wd4820 # Padding
			/wd4710 # Function was not inlined
			/wd5045 # Spectre mitigations
			/wd4711 # Function selected for automatic inlining
			/wd4706 # Assignment inside conditional expression (even with double paren)
			/wd4061 # Non-explicitly handled enum value (Doesn't count `default:')
			/wd4127) # Conditional expression is constant (Even if block contains a `break')
