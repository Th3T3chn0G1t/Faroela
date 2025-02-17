# SPDX-License-Identifier: MIT

add_library(faroela-api INTERFACE)

target_compile_features(faroela-api INTERFACE cxx_std_23)

set(
		FAROELA_API_HEADERS
		faroela.hpp
		types.hpp)

list(TRANSFORM FAROELA_API_HEADERS PREPEND ${CMAKE_CURRENT_LIST_DIR}/include/faroela/api/)

target_sources(faroela-api INTERFACE ${FAROELA_API_HEADERS} ${CMAKE_CURRENT_LIST_DIR}/src/null.cpp)

target_include_directories(faroela-api INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include/)
