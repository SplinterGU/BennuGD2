project(mod_misc)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "../bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/")

set(CMAKE_MODULE_PATH ../cmake)

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS} ../../core/include ../../core/bgdrtm ../../modules/libmod_misc)

file(GLOB SOURCES_LIBMOD_MISC
     "../../modules/libmod_misc/*.c"
     )

add_library(mod_misc SHARED ${SOURCES_LIBMOD_MISC})
target_link_libraries(mod_misc ${SDL2_LIBRARY} ${SDL2_LIBRARIES} -L../bin -lbgdrtm)
