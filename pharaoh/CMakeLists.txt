# SPDX-License-Identifier: MIT

include(ExternalProject)

set(PHARAOH_BINDIR ${CMAKE_CURRENT_BINARY_DIR})

add_library(faroela-module MODULE)

target_link_libraries(faroela-module PRIVATE $<LINK_LIBRARY:WHOLE_ARCHIVE,faroela>)

set_target_properties(
		faroela-module PROPERTIES
		LIBRARY_OUTPUT_DIRECTORY ${PHARAOH_BINDIR}
		OUTPUT_NAME faroela
		PREFIX "")

ExternalProject_Add(
		pharaoh
		CONFIGURE_COMMAND ""
		INSTALL_COMMAND ""
		DEPENDS faroela-module
		SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}
		BUILD_ALWAYS true
		BUILD_COMMAND
		dotnet build ${CMAKE_CURRENT_LIST_DIR}/pharaoh.csproj
		--artifacts-path ${PHARAOH_BINDIR}
		--output ${PHARAOH_BINDIR}
		--self-contained $<IF:$<CONFIG:Debug>,true,false>)
