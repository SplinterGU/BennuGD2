project(bginput)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "../../bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../../bin/")

set(CMAKE_MODULE_PATH ../../cmake/Modules)

find_package(SDL2 REQUIRED)
find_package(SDL_GPU REQUIRED)

add_definitions(-D__LIBBGINPUT)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS} ../../core/include ../../core/bgdrtm ../../modules/libbginput ../../modules/libbggfx ${SDL_GPU_INCLUDE_DIR})

file(GLOB SOURCES_LIBBGLOAD
     "../../modules/libbginput/*.c"
     )

add_library(bginput SHARED ${SOURCES_LIBBGLOAD})
target_link_libraries(bginput ${SDL2_LIBRARY} ${SDL2_LIBRARIES} -L../../bin -lbggfx -lbgdrtm)
