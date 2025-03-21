# SPDX-License-Identifier: MIT

include(CheckCXXCompilerFlag)
include(CMakeParseArguments)

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

### Add default CXX diagnostic flags.
function(cxx_default_diagnostics TARGET)
	cxx_options_conditional(
			COMMAND target_compile_options
			ARGS ${TARGET} PRIVATE
			FLAGS -Wall -Wextra -Werror -pedantic -pedantic-errors)

	cxx_options_conditional(
			COMMAND target_compile_options
			ARGS ${TARGET} PRIVATE
			FLAGS /Wall /WX
			/wd4820 # Padding
			/wd4710 # Function was not inlined
			/wd5045 # Spectre mitigations
			/wd4514 # Unreferenced inline function has been removed
			/wd4711 # Function selected for automatic inlining
			/wd4706 # Assignment inside conditional expression (even with double paren)
			/wd4061 # Non-explicitly handled enum value (Doesn't count `default:')
			/wd4623 # Default constructor was implicitly defined as deleted
			/wd4868 # Compiler may not enforce left-to-right evaluation order in braced initializer list
			/wd4127 # Conditional expression is constant (Even if block contains a `break')
			/wd4625 # Copy constructor was implicitly defined as deleted
			/wd4626) # Assignment operator was implicitly defined as deleted
endfunction()
