# SPDX-License-Identifier: MIT

include(CheckCXXCompilerFlag)
include(CMakeParseArguments)

function(target_compile_link_options TARGET VISIBILITY)
	foreach(FLAG IN LISTS ${ARGN})
		target_link_options(${TARGET} ${VISIBILITY} ${FLAG})
		target_compile_options(${TARGET} ${VISIBILITY} ${FLAG})
	endforeach()
endfunction()

### Convert compile flag to variable-appropriate string.
### e.g. "-Wall" -> "<prefix>_WALL<suffix>".
function(flag_string_variable FLAG OUT_VAR PREFIX SUFFIX)
	string(SUBSTRING ${FLAG} 0 1 _FLAG_START)
	string(SUBSTRING ${FLAG} 1 -1 _FLAG_NAME)

	# MS to GNU style CLI.
	if(${_FLAG_START} STREQUAL "/")
		string(PREPEND _FLAG_NAME "_MS_")
	else()
		string(PREPEND _FLAG_NAME "_")
	endif()

	string(REPLACE "+" "X" _FLAG_NAME ${_FLAG_NAME})
	string(MAKE_C_IDENTIFIER ${_FLAG_NAME} _FLAG_NAME)

	string(TOUPPER "${_FLAG_NAME}" _FLAG_NAME)

	set(${OUT_VAR} "${PREFIX}${_FLAG_NAME}${SUFFIX}" PARENT_SCOPE)
endfunction()

### Add compile flag if it exists.
function(cxx_options_conditional)
	cmake_parse_arguments(
			CXX_OPTIONS_CONDITIONAL
			"REQUIRED" "COMMAND" "ARGS;FLAGS"
			${ARGN})

	foreach(FLAG IN LISTS CXX_OPTIONS_CONDITIONAL_FLAGS)
		flag_string_variable(${FLAG} _FLAG_NAME "CXX_FLAG" "")
		check_cxx_compiler_flag(${FLAG} ${_FLAG_NAME})

		if(${${_FLAG_NAME}})
			cmake_language(
					CALL
					${CXX_OPTIONS_CONDITIONAL_COMMAND}
					${CXX_OPTIONS_CONDITIONAL_ARGS}
					${FLAG})
		elseif(${CXX_OPTIONS_CONDITIONAL_REQUIRED})
			message(FATAL_ERROR "${_FLAG_NAME} Required")
		endif()
	endforeach()
endfunction()

### Calls `cxx_options_conditional` on `target_compile_link_options`,
### But applies multiple args at once and enforces the flags during the
### Link stage of the test.
function(target_cxx_multi_compile_link_options_conditional TARGET VISIBILITY)
	list(JOIN ARGN " " FLAGS)

	set(CXX_FLAG_STORE ${CMAKE_CXX_FLAGS})
	set(CXX_EXE_FLAG_STORE ${CMAKE_EXE_LINKER_FLAGS})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS}")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${FLAGS}")

	# TODO: This should only apply the args if all flags pass the feature test.
	cxx_options_conditional(
			COMMAND target_compile_link_options
			ARGS TWD PRIVATE
			FLAGS ${ARGN})

	set(CMAKE_CXX_FLAGS ${CXX_FLAG_STORE})
	set(CMAKE_EXE_LINKER_FLAGS ${CXX_EXE_FLAG_STORE})
endfunction()
