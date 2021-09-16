project(bggfx)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/Modules/common_defs.cmake)

add_definitions(-D__LIBBGFGX)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS} ../../core/include ../../core/bgdrtm ../../modules/libbggfx ${SDL_GPU_INCLUDE_DIR})

file(GLOB SOURCES_LIBBGGFX
     "../../modules/libbggfx/*.c"
     )

if(NOT USE_VLC)
	list(REMOVE_ITEM SOURCES_LIBBGGFX "${CMAKE_CURRENT_SOURCE_DIR}/../../modules/libbggfx/g_media.c")
endif()

add_library(bggfx ${LIB_TYPE} ${SOURCES_LIBBGGFX})
if(MINGW)
set(OGL_LIB -lopengl32)
endif()
target_link_libraries(bggfx ${SDL2_LIBRARY} ${SDL2_LIBRARIES} ${SDL_GPU_LIBRARY} -L../../bin bgdrtm ${OGL_LIB})
if(USE_VLC)
	target_link_libraries(-lvlc)
endif()
