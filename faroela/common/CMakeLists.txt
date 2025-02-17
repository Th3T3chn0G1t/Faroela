# SPDX-License-Identifier: MIT

add_subdirectory(vendor)

add_library(faroela-common STATIC)

target_compile_features(faroela-common PUBLIC cxx_std_23)

set(
		FAROELA_COMMON_SOURCES
		log.cpp
		result.cpp)

set(
		FAROELA_COMMON_HEADERS
		formatters.hpp
		log.hpp
		result.hpp)

list(TRANSFORM FAROELA_COMMON_SOURCES PREPEND ${CMAKE_CURRENT_LIST_DIR}/src/)
list(TRANSFORM FAROELA_COMMON_HEADERS PREPEND ${CMAKE_CURRENT_LIST_DIR}/include/faroela/common/)

target_sources(faroela-common PRIVATE ${FAROELA_COMMON_SOURCES} ${FAROELA_COMMON_HEADERS})

target_precompile_headers(faroela-common PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include/faroela/common/pch.hpp)

target_include_directories(faroela-common PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include/)

set_target_properties(
		faroela-common PROPERTIES
		CXX_VISIBILITY_PRESET hidden)

target_link_libraries(faroela-common PUBLIC spdlog magic_enum)

cxx_default_diagnostics(faroela-common)
