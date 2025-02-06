# SPDX-License-Identifier: MIT

add_subdirectory(vendor)

add_library(faroela STATIC)

target_compile_features(faroela PUBLIC cxx_std_23)

set(
		FAROELA_SOURCES
		api.cpp
		core.cpp
		hid.cpp
		log.cpp
		result.cpp)

set(
		FAROELA_HEADERS
		core.hpp
		formatters.hpp
		hid.hpp
		log.hpp
		result.hpp)

list(TRANSFORM FAROELA_SOURCES PREPEND ${CMAKE_CURRENT_LIST_DIR}/src/)
list(TRANSFORM FAROELA_HEADERS PREPEND ${CMAKE_CURRENT_LIST_DIR}/include/faroela/)

target_sources(faroela PRIVATE ${FAROELA_SOURCES} ${FAROELA_HEADERS})

# TODO: Make faroela-api an interface target.
set(
		FAROELA_API_HEADERS
		faroela.hpp
		types.hpp)

list(TRANSFORM FAROELA_API_HEADERS PREPEND ${CMAKE_CURRENT_LIST_DIR}/include/faroela-api/)

target_sources(faroela PUBLIC ${FAROELA_API_HEADERS})

target_precompile_headers(faroela PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include/faroela/pch.hpp)

target_include_directories(faroela PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include/)
target_include_directories(faroela PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include/)

set_target_properties(
		faroela PROPERTIES
		CXX_VISIBILITY_PRESET hidden)

target_link_libraries(faroela PRIVATE spdlog Tracy::TracyClient magic_enum)

cxx_default_diagnostics(faroela)
