# SPDX-License-Identifier: MIT

include(ExternalProject)

set(PHARO_BINDIR ${CMAKE_CURRENT_BINARY_DIR})

ExternalProject_Add(
        pharaoh
        CONFIGURE_COMMAND ""
        INSTALL_COMMAND ""
        SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}
        BUILD_ALWAYS true
        BUILD_COMMAND
        dotnet build ${CMAKE_CURRENT_LIST_DIR}/pharaoh.csproj
        --artifacts-path ${PHARO_BINDIR}
        --output ${PHARO_BINDIR}
        --self-contained $<IF:$<CONFIG:Debug>,true,false>)

add_dependencies(pharaoh faroela)

add_custom_command(
        TARGET pharaoh POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:faroela> ${PHARO_BINDIR})
