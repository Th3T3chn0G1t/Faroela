# SPDX-License-Identifier: MIT

set(SDL_DEPS_SHARED OFF)
set(SDL_STATIC ON)
set(SDL_SHARED OFF)
set(SDL_TEST_LIBRARY OFF)
set(SDL_EXAMPLES OFF)
set(SDL_AUDIO OFF)
set(SDL_VIDEO ON)
set(SDL_GPU OFF)
set(SDL_RENDER OFF)
set(SDL_CAMERA OFF)
set(SDL_JOYSTICK ON)
set(SDL_HAPTIC ON)
set(SDL_HIDAPI ON)
set(SDL_POWER OFF)
set(SDL_SENSOR OFF)
set(SDL_DIALOG OFF)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/sdl SYSTEM)
