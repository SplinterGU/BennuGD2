cmake_minimum_required(VERSION 2.8.9)
project(bennugd2)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/common_defs.cmake)

# core
add_subdirectory(core/bgdc)
add_subdirectory(core/bgdrtm)
add_subdirectory(core/bgdi)

# modules
add_subdirectory(modules/libbgload)
add_subdirectory(modules/libsdlhandler)
add_subdirectory(modules/libbggfx)
add_subdirectory(modules/libbginput)
add_subdirectory(modules/libmod_gfx)
add_subdirectory(modules/libmod_misc)
add_subdirectory(modules/libmod_input)
add_subdirectory(modules/libmod_sound)
add_subdirectory(modules/libmod_debug)

#tools
add_subdirectory(tools/moddesc)
