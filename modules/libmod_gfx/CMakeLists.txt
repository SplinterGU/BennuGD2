project(mod_gfx)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/${TARGET}/bin/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/${TARGET}/bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/${TARGET}/bin/")

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
if(USE_SDL2_GPU)
find_package(SDL_GPU REQUIRED)
endif()

add_definitions(${EXTRA_CFLAGS})

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_image_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/core/include ${CMAKE_SOURCE_DIR}/core/bgdrtm ${CMAKE_SOURCE_DIR}/modules/libbggfx ${CMAKE_SOURCE_DIR}/modules/libmod_gfx ${CMAKE_SOURCE_DIR}/modules/libbgload ${CMAKE_SOURCE_DIR}/modules/libbginput ${SDL_GPU_INCLUDE_DIR} ${INCLUDE_DIRECTORIES})

file(GLOB SOURCES_LIBMOD_GFX
     "${CMAKE_SOURCE_DIR}/modules/libmod_gfx/*.c"
     )

add_library(mod_gfx ${LIBRARY_BUILD_TYPE} ${SOURCES_LIBMOD_GFX})

target_link_libraries(mod_gfx ${SDL2_LIBRARY} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARY} ${SDL2_image_LIBRARIES} ${SDL_GPU_LIBRARY} -L${CMAKE_SOURCE_DIR}/bin bgdrtm bggfx bgload)
