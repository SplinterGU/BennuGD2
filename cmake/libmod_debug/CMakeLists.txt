project(mod_debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "../bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/")

set(CMAKE_MODULE_PATH ../cmake)

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS} ../../core/include ../../core/bgdrtm ../../modules/libbggfx ../../modules/libmod_debug ../../modules/libbginput)

file(GLOB SOURCES_LIBMOD_DEBUG
     "../../modules/libmod_debug/*.c"
     )

add_library(mod_debug SHARED ${SOURCES_LIBMOD_DEBUG})
target_link_libraries(mod_debug ${SDL2_LIBRARY} ${SDL2_LIBRARIES} -L../bin -lbgdrtm -lbggfx -lbginput)
