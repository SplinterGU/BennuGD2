project(bginput)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "../bin/")

set(CMAKE_MODULE_PATH ../cmake)

find_package(SDL2 2.0.4 REQUIRED)

add_definitions(-DTARGET_LINUX -D__LIBBGINPUT)

include_directories(${SDL2_INCLUDE_DIR} ../../core/include ../../core/bgdrtm ../../modules/libbginput ../../modules/libbggfx)

file(GLOB SOURCES_LIBBGLOAD
     "../../modules/libbginput/*.c"
     )

add_library(bginput SHARED ${SOURCES_LIBBGLOAD})
target_link_libraries(bginput ${SDL2_LIBRARY} -L../bin -lbggfx -lbgdrtm)
