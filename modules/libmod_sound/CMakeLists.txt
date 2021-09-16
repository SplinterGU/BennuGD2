project(mod_sound)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/Modules/common_defs.cmake)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS} ${SDLMIXER_INCLUDE_DIRS} ${SDL2_mixer_INCLUDE_DIRS} ../../core/include ../../core/bgdrtm ../../modules/libmod_sound ../../modules/libbgload)

file(GLOB SOURCES_LIBMOD_SOUND
     "../../modules/libmod_sound/*.c"
     )

add_library(mod_sound ${LIB_TYPE} ${SOURCES_LIBMOD_SOUND})
target_link_libraries(mod_sound ${SDL2_LIBRARY} ${SDL2_LIBRARIES} ${SDLMIXER_LIBRARY} ${SDL2_mixer_LIBRARIES} -L../../bin bgdrtm bgload)
