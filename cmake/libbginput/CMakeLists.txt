project(bginput)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "../bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/")

set(CMAKE_MODULE_PATH ../cmake)

find_package(SDL2 REQUIRED)

add_definitions(-D__LIBBGINPUT)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS} ../../core/include ../../core/bgdrtm ../../modules/libbginput ../../modules/libbggfx)

file(GLOB SOURCES_LIBBGLOAD
     "../../modules/libbginput/*.c"
     )

add_library(bginput SHARED ${SOURCES_LIBBGLOAD})
target_link_libraries(bginput ${SDL2_LIBRARY} ${SDL2_LIBRARIES} -L../bin -lbggfx -lbgdrtm)
