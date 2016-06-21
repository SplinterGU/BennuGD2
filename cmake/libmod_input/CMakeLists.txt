project(mod_input)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "../bin/")

set(CMAKE_MODULE_PATH ../cmake)

find_package(SDL2 2.0.4 REQUIRED)

add_definitions(-DTARGET_LINUX)

include_directories(${SDL2_INCLUDE_DIR} ../../core/include ../../core/bgdrtm/include ../../modules/libbggfx  ../../modules/libbginput ../../modules/libmod_input)

file(GLOB SOURCES_LIBMOD_INPUT
     "../../modules/libmod_input/*.c"
     )

add_library(mod_input SHARED ${SOURCES_LIBMOD_INPUT})
target_link_libraries(mod_input ${SDL2_LIBRARY} -L../bin -lbgdrtm -lbggfx -lbginput)
