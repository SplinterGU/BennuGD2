project(bggfx)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "../bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/")

set(CMAKE_MODULE_PATH ../cmake)

find_package(SDL2 REQUIRED)

add_definitions(-D__LIBBGFGX)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS} ../../core/include ../../core/bgdrtm ../../modules/libbggfx)

file(GLOB SOURCES_LIBBGGFX
     "../../modules/libbggfx/*.c"
     )

add_library(bggfx SHARED ${SOURCES_LIBBGGFX})
target_link_libraries(bggfx ${SDL2_LIBRARY} ${SDL2_LIBRARIES} -L../bin -lbgdrtm)
