cmake_minimum_required(VERSION 2.8.9)
project(bennugd)

# core
add_subdirectory(bgdc)
add_subdirectory(bgdrtm)
add_subdirectory(bgdi)

# modules
add_subdirectory(libbgload)
add_subdirectory(libsdlhandler)
add_subdirectory(libbggfx)
add_subdirectory(libbginput)
add_subdirectory(libmod_gfx)
add_subdirectory(libmod_misc)
add_subdirectory(libmod_input)
add_subdirectory(libmod_sound)

#tools
add_subdirectory(moddesc)
