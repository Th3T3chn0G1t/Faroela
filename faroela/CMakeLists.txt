# SPDX-License-Identifier: MIT

add_subdirectory(common)
add_subdirectory(api)

add_subdirectory(vendor)

add_library(faroela STATIC)

target_compile_features(faroela PUBLIC cxx_std_23)

set(
		FAROELA_SOURCES
		api.cpp
		core.cpp
		hid.cpp
		render.cpp)

set(
		FAROELA_HEADERS
		core.hpp
		delegate.hpp
		hid.hpp
		platform.hpp
		render.hpp)

list(TRANSFORM FAROELA_SOURCES PREPEND ${CMAKE_CURRENT_LIST_DIR}/src/)
list(TRANSFORM FAROELA_HEADERS PREPEND ${CMAKE_CURRENT_LIST_DIR}/include/faroela/)

if(WIN32)
	include(src/windows/CMakeLists.txt)
else()
	include(src/nix_generic/CMakeLists.txt)
endif()

target_sources(faroela PRIVATE ${FAROELA_SOURCES} ${FAROELA_HEADERS})

target_precompile_headers(faroela PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include/faroela/pch.hpp)

target_include_directories(faroela PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include/)

set_target_properties(
		faroela PROPERTIES
		CXX_VISIBILITY_PRESET hidden)

target_link_libraries(faroela PUBLIC faroela-common faroela-api)
target_link_libraries(faroela PRIVATE Tracy::TracyClient libuv::libuv unordered_dense function-ref bgfx)

cxx_default_diagnostics(faroela)
