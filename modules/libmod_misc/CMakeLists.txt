project(mod_misc)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "../../bin/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "../../bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../../bin/")

set(CMAKE_MODULE_PATH ../../cmake/Modules)

find_package(SDL2 REQUIRED)

add_definitions(${EXTRA_CFLAGS})

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS} ../../core/include ../../core/bgdrtm ../../modules/libmod_misc)

file(GLOB SOURCES_LIBMOD_MISC
     "../../modules/libmod_misc/*.c"
     )

add_library(mod_misc ${LIBRARY_BUILD_TYPE} ${SOURCES_LIBMOD_MISC})

target_link_libraries(mod_misc ${SDL2_LIBRARY} ${SDL2_LIBRARIES} -L../../bin bgdrtm)
