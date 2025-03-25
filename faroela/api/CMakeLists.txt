# SPDX-License-Identifier: MIT

set(FAROELA_API_GENERATED ${CMAKE_CURRENT_LIST_DIR}/include/faroela/api/faroela.hpp)

add_custom_command(
		OUTPUT ${FAROELA_API_GENERATED}
		CODEGEN
		COMMAND
			${FAROELA_PYTHON_INTERPRETER}
			ARGS
			${CMAKE_CURRENT_LIST_DIR}/generate.py
			-i ${CMAKE_CURRENT_LIST_DIR}/api.json
			-o ${FAROELA_API_GENERATED}
			-t c++
		VERBATIM
		WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
		MAIN_DEPENDENCY ${CMAKE_CURRENT_LIST_DIR}/generate.py
		DEPENDS
			${CMAKE_CURRENT_LIST_DIR}/api.json
			${CMAKE_CURRENT_LIST_DIR}/extra/cpp_head.hpp
			${CMAKE_CURRENT_LIST_DIR}/extra/cpp_tail.hpp)

if(CMAKE_VERSION VERSION_LESS "3.30")
	add_library(faroela-api STATIC)
	target_compile_features(faroela-api PUBLIC cxx_std_23)
	target_sources(faroela-api PUBLIC ${FAROELA_API_GENERATED} ${CMAKE_CURRENT_LIST_DIR}/src/null.cpp)
else()
	add_library(faroela-api INTERFACE)
	target_compile_features(faroela-api INTERFACE cxx_std_23)
	target_sources(faroela-api INTERFACE ${FAROELA_API_GENERATED})
endif()

target_include_directories(faroela-api INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include/)
