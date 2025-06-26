project(mod_net)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/${TARGET}/bin/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/${TARGET}/bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/${TARGET}/bin/")

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)

find_package(SDL2 REQUIRED)

add_definitions(${EXTRA_CFLAGS})

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/core/include ${CMAKE_SOURCE_DIR}/core/bgdrtm ${CMAKE_SOURCE_DIR}/modules/libmod_net ${INCLUDE_DIRECTORIES})

file(GLOB SOURCES_LIBMOD_net
     "${CMAKE_SOURCE_DIR}/modules/libmod_net/*.c"
     )

add_library(mod_net ${LIBRARY_BUILD_TYPE} ${SOURCES_LIBMOD_net})

target_link_libraries(mod_net ${SDL2_LIBRARY} ${SDL2_LIBRARIES} -L${CMAKE_SOURCE_DIR}/bin bgdrtm)
if (WIN32)
    add_definitions(-D_WIN32_WINNT=0x0600)
    target_link_libraries(mod_net ws2_32)
endif()
if (SWITCH)
    target_link_libraries(mod_net nx)
endif()
