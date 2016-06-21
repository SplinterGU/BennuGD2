project(sdlhandler)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "../bin/")

set(CMAKE_MODULE_PATH ../cmake)

find_package(SDL2 2.0.4 REQUIRED)

add_definitions(-DTARGET_LINUX)

include_directories(${SDL2_INCLUDE_DIR} ../../core/include ../../core/bgdrtm/include ../../modules/libsdlhandler)

file(GLOB SOURCES_LIBSDLHANDLER
     "../../modules/libsdlhandler/*.c"
     )

add_library(sdlhandler SHARED ${SOURCES_LIBSDLHANDLER})
target_link_libraries(sdlhandler ${SDL2_LIBRARY} -L../bin -lbgdrtm)
