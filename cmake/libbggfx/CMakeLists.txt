project(bggfx)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "../bin/")

set(CMAKE_MODULE_PATH ../cmake)

find_package(SDL2 2.0.4 REQUIRED)

add_definitions(-DTARGET_LINUX -D__LIBBGFGX)

include_directories(${SDL2_INCLUDE_DIR} ../../core/include ../../core/bgdrtm ../../modules/libbggfx)

file(GLOB SOURCES_LIBBGGFX
     "../../modules/libbggfx/*.c"
     )

add_library(bggfx SHARED ${SOURCES_LIBBGGFX})
target_link_libraries(bggfx ${SDL2_LIBRARY} -L../bin -lbgdrtm)
