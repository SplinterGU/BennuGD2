project(mod_gfx)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/Modules/common_defs.cmake)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_image_INCLUDE_DIRS} ../../core/include ../../core/bgdrtm ../../modules/libbggfx ../../modules/libmod_gfx ../../modules/libbgload ../../modules/libbginput ${SDL_GPU_INCLUDE_DIR})

file(GLOB SOURCES_LIBMOD_GFX
     "../../modules/libmod_gfx/*.c"
     )
	 
if(NOT USE_VLC)
	list(REMOVE_ITEM SOURCES_LIBMOD_GFX "${CMAKE_CURRENT_SOURCE_DIR}/../../modules/libmod_gfx/m_media.c")
endif()


add_library(mod_gfx ${LIB_TYPE} ${SOURCES_LIBMOD_GFX})
target_link_libraries(mod_gfx ${SDL2_LIBRARY} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARY} ${SDL2_image_LIBRARIES} ${SDL_GPU_LIBRARY} -L../../bin bgdrtm bggfx bgload)
