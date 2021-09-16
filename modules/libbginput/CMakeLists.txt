project(bginput)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/Modules/common_defs.cmake)

add_definitions(-D__LIBBGINPUT)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS} ../../core/include ../../core/bgdrtm ../../modules/libbginput ../../modules/libbggfx ${SDL_GPU_INCLUDE_DIR})

file(GLOB SOURCES_LIBBGINPUT
     "../../modules/libbginput/*.c"
     )

add_library(bginput ${LIB_TYPE} ${SOURCES_LIBBGINPUT})
target_link_libraries(bginput ${SDL2_LIBRARY} ${SDL2_LIBRARIES} -L../../bin bggfx bgdrtm)
