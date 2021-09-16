project(mod_debug)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/Modules/common_defs.cmake)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS} ../../core/include ../../core/bgdrtm ../../modules/libbggfx ../../modules/libmod_debug ../../modules/libbginput ${SDL_GPU_INCLUDE_DIR})

file(GLOB SOURCES_LIBMOD_DEBUG
     "../../modules/libmod_debug/*.c"
     )

add_library(mod_debug ${LIB_TYPE} ${SOURCES_LIBMOD_DEBUG})
target_link_libraries(mod_debug ${SDL2_LIBRARY} ${SDL2_LIBRARIES} -L../../bin bgdrtm bggfx bginput)
