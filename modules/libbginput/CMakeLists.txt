project(bginput)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "../../bin/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "../../bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../../bin/")

set(CMAKE_MODULE_PATH ../../cmake/Modules)

find_package(SDL2 REQUIRED)
if($USE_SDL2_GPU)
find_package(SDL_GPU REQUIRED)
endif()

add_definitions(-D__LIBBGINPUT ${EXTRA_CFLAGS})

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS} ../../core/include ../../core/bgdrtm ../../modules/libbginput ../../modules/libbggfx ${SDL_GPU_INCLUDE_DIR} ${INCLUDE_DIRECTORIES})

file(GLOB SOURCES_LIBBGINPUT
     "../../modules/libbginput/*.c"
     )

add_library(bginput ${LIBRARY_BUILD_TYPE} ${SOURCES_LIBBGINPUT})

target_link_libraries(bginput ${SDL2_LIBRARY} ${SDL2_LIBRARIES} -L../../bin bggfx bgdrtm)
