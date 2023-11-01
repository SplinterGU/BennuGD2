project(mod_sound)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "../../bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../../bin/")

set(CMAKE_MODULE_PATH ../../cmake/Modules)

find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS} ${SDLMIXER_INCLUDE_DIRS} ${SDL2_mixer_INCLUDE_DIRS} ../../core/include ../../core/bgdrtm ../../modules/libmod_sound ../../modules/libbgload)

file(GLOB SOURCES_LIBMOD_SOUND
     "../../modules/libmod_sound/*.c"
     )

add_library(mod_sound SHARED ${SOURCES_LIBMOD_SOUND})
target_link_libraries(mod_sound ${SDL2_LIBRARY} ${SDL2_LIBRARIES} ${SDLMIXER_LIBRARY} ${SDL2_mixer_LIBRARIES} -L../../bin bgdrtm bgload)
