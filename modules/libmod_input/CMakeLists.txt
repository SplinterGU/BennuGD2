project(mod_input)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/${TARGET}/bin/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/${TARGET}/bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/${TARGET}/bin/")

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)

find_package(SDL2 REQUIRED)

add_definitions(${EXTRA_CFLAGS})

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/core/include ${CMAKE_SOURCE_DIR}/core/bgdrtm ${CMAKE_SOURCE_DIR}/modules/libbggfx  ${CMAKE_SOURCE_DIR}/modules/libbginput ${CMAKE_SOURCE_DIR}/modules/libmod_input ${INCLUDE_DIRECTORIES})

file(GLOB SOURCES_LIBMOD_INPUT
     "${CMAKE_SOURCE_DIR}/modules/libmod_input/*.c"
     )

add_library(mod_input ${LIBRARY_BUILD_TYPE} ${SOURCES_LIBMOD_INPUT})

target_link_libraries(mod_input ${SDL2_LIBRARY} ${SDL2_LIBRARIES} -L${CMAKE_SOURCE_DIR}/bin bgdrtm bggfx bginput)
