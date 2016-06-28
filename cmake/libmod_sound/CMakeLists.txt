project(mod_sound)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "../bin/")

set(CMAKE_MODULE_PATH ../cmake)

find_package(SDL2 2.0.4 REQUIRED)
find_package(SDL2_mixer REQUIRED)

add_definitions(-DTARGET_LINUX)

include_directories(${SDL2_INCLUDE_DIR} ${SDLMIXER_INCLUDE_DIRS} ../../core/include ../../core/bgdrtm ../../modules/libmod_sound ../../modules/libbgload)

file(GLOB SOURCES_LIBMOD_SOUND
     "../../modules/libmod_sound/*.c"
     )

add_library(mod_sound SHARED ${SOURCES_LIBMOD_SOUND})
target_link_libraries(mod_sound ${SDL2_LIBRARY} ${SDLMIXER_LIBRARY} -L../bin -lbgdrtm -lbgload)
