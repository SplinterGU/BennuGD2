project(mod_sound)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/${TARGET}/bin/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/${TARGET}/bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/${TARGET}/bin/")

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)

find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)

add_definitions(${EXTRA_CFLAGS})

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS} ${SDLMIXER_INCLUDE_DIRS} ${SDL2_mixer_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/core/include ${CMAKE_SOURCE_DIR}/core/bgdrtm ${CMAKE_SOURCE_DIR}/modules/libmod_sound ${CMAKE_SOURCE_DIR}/modules/libbgload ${INCLUDE_DIRECTORIES})

file(GLOB SOURCES_LIBMOD_SOUND
     "${CMAKE_SOURCE_DIR}/modules/libmod_sound/*.c"
     )

add_library(mod_sound ${LIBRARY_BUILD_TYPE} ${SOURCES_LIBMOD_SOUND})

if(ANDROID)
    add_definitions(-D__ANDROID__)
    include_directories(/usr/src/SDL2_mixer/include)
    set(ANDROID_PATH_LIB -L/usr/src/SDL2_mixer/lib)
endif()

target_link_libraries(mod_sound ${SDL2_LIBRARY} ${SDL2_LIBRARIES} ${SDLMIXER_LIBRARY} ${SDL2_mixer_LIBRARIES} -L${CMAKE_SOURCE_DIR}/bin bgdrtm bgload)
