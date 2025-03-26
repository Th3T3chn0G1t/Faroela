# SPDX-License-Identifier: MIT

function(faroela_api_generate LANGUAGE OUTPUT)
	set(FAROELA_API_GENERATE_SCRIPT ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/generate.py)
	set(FAROELA_API_SOURCE ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/api.json)

	add_custom_target(
			faroela-api-generated-${LANGUAGE}
			COMMAND
				${FAROELA_PYTHON_INTERPRETER}
				${FAROELA_API_GENERATE_SCRIPT}
				-i ${FAROELA_API_SOURCE}
				-o ${OUTPUT}
				-t ${LANGUAGE}
			DEPENDS
				${FAROELA_API_GENERATE_SCRIPT}
				${CMAKE_CURRENT_FUNCTION_LIST_DIR}/extra/${LANGUAGE}/head
				${CMAKE_CURRENT_FUNCTION_LIST_DIR}/extra/${LANGUAGE}/tail
			BYPRODUCTS
				${OUTPUT}
			WORKING_DIRECTORY
				${CMAKE_CURRENT_FUNCTION_LIST_DIR})

	# TODO: This should be the correct pattern but appears to be broken <3.30.
	#add_custom_command(
	#		OUTPUT ${FAROELA_API_GENERATED}
	#		$<$<VERSION_GREATER_EQUAL:${CMAKE_VERSION},"3.30">:CODEGEN>
	#		COMMAND
	#			${FAROELA_PYTHON_INTERPRETER}
	#			ARGS
	#			${CMAKE_CURRENT_LIST_DIR}/generate.py
	#			-i ${CMAKE_CURRENT_LIST_DIR}/api.json
	#			-o ${FAROELA_API_GENERATED}
	#			-t c++
	#		VERBATIM
	#		WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
	#		MAIN_DEPENDENCY ${CMAKE_CURRENT_LIST_DIR}/generate.py
	#		DEPENDS
	#			${CMAKE_CURRENT_LIST_DIR}/api.json
	#			${CMAKE_CURRENT_LIST_DIR}/extra/cpp_head.hpp
	#			${CMAKE_CURRENT_LIST_DIR}/extra/cpp_tail.hpp)
endfunction()

add_library(faroela-api INTERFACE)
target_compile_features(faroela-api INTERFACE cxx_std_23)
target_include_directories(faroela-api INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include/)

set(FAROELA_API_GENERATED ${CMAKE_CURRENT_LIST_DIR}/include/faroela/api/faroela.hpp)
faroela_api_generate(cxx ${FAROELA_API_GENERATED})

add_dependencies(faroela-api faroela-api-generated-cxx)
target_sources(faroela-api INTERFACE ${FAROELA_API_GENERATED})
