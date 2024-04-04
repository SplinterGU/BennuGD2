project(mod_net)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "../../bin/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "../../bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../../bin/")

set(CMAKE_MODULE_PATH ../../cmake/Modules)

find_package(SDL2 REQUIRED)

add_definitions(${EXTRA_CFLAGS})

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS} ../../core/include ../../core/bgdrtm ../../modules/libmod_net ${INCLUDE_DIRECTORIES})

file(GLOB SOURCES_LIBMOD_net
     "../../modules/libmod_net/*.c"
     )

add_library(mod_net ${LIBRARY_BUILD_TYPE} ${SOURCES_LIBMOD_net})

target_link_libraries(mod_net ${SDL2_LIBRARY} ${SDL2_LIBRARIES} -L../../bin bgdrtm)
if (WIN32)
    target_link_libraries(mod_net ws2_32)
endif()
if (SWITCH)
    target_link_libraries(mod_net nx)
endif()
