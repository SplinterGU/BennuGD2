project(mod_debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "../../bin/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "../../bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../../bin/")

set(CMAKE_MODULE_PATH ../../cmake/Modules)

find_package(SDL2 REQUIRED)
if(USE_SDL2_GPU)
find_package(SDL_GPU REQUIRED)
endif()

add_definitions(${EXTRA_CFLAGS})

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS} ../../core/include ../../core/bgdrtm ../../modules/libbggfx ../../modules/libmod_debug ../../modules/libbginput ${SDL_GPU_INCLUDE_DIR})

file(GLOB SOURCES_LIBMOD_DEBUG
     "../../modules/libmod_debug/*.c"
     )

add_library(mod_debug ${LIBRARY_BUILD_TYPE} ${SOURCES_LIBMOD_DEBUG})

target_link_libraries(mod_debug ${SDL2_LIBRARY} ${SDL2_LIBRARIES} -L../../bin bgdrtm bggfx bginput)
