# SPDX-License-Identifier: MIT

# TODO: SPDLOG_NO_EXCEPTIONS
set(SPDLOG_USE_STD_FORMAT ON)
set(SPDLOG_ENABLE_PCH ON)
set(SPDLOG_MSVC_UTF8 ON)
set(SPDLOG_BUILD_PIC ON)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/spdlog SYSTEM)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/magic_enum SYSTEM)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/helper/eternal SYSTEM)
