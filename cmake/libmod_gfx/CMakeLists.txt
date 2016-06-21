project(mod_gfx)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "../bin/")

set(CMAKE_MODULE_PATH ../cmake)

find_package(SDL2 2.0.4 REQUIRED)
find_package(SDL2_image REQUIRED)

add_definitions(-DTARGET_LINUX)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ../../core/include ../../core/bgdrtm/include ../../modules/libbggfx ../../modules/libmod_gfx ../../modules/libbgload)

file(GLOB SOURCES_LIBMOD_GFX
     "../../modules/libmod_gfx/*.c"
     )

add_library(mod_gfx SHARED ${SOURCES_LIBMOD_GFX})
target_link_libraries(mod_gfx ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} -L../bin -lbgdrtm -lbggfx -lbgload)
