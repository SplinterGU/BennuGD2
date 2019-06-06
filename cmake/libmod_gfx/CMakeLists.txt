project(mod_gfx)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "../bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/")

set(CMAKE_MODULE_PATH ../cmake)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_image_INCLUDE_DIRS} ../../core/include ../../core/bgdrtm ../../modules/libbggfx ../../modules/libmod_gfx ../../modules/libbgload)

file(GLOB SOURCES_LIBMOD_GFX
     "../../modules/libmod_gfx/*.c"
     )

add_library(mod_gfx SHARED ${SOURCES_LIBMOD_GFX})
target_link_libraries(mod_gfx ${SDL2_LIBRARY} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARY} ${SDL2_image_LIBRARIES} -L../bin -lbgdrtm -lbggfx -lbgload)
