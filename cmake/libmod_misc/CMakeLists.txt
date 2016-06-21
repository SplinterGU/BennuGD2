project(mod_misc)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "../bin/")

set(CMAKE_MODULE_PATH ../cmake)

find_package(SDL2 2.0.4 REQUIRED)

add_definitions(-DTARGET_LINUX)

include_directories(${SDL2_INCLUDE_DIR} ../../core/include ../../core/bgdrtm/include ../../modules/libmod_misc)

file(GLOB SOURCES_LIBMOD_MISC
     "../../modules/libmod_misc/*.c"
     )

add_library(mod_misc SHARED ${SOURCES_LIBMOD_MISC})
target_link_libraries(mod_misc ${SDL2_LIBRARY} -L../bin -lbgdrtm)
