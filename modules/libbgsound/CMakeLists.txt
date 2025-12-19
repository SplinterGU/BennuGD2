project(bgsound)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/${TARGET}/bin/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/${TARGET}/bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/${TARGET}/bin/")

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)

find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)

add_definitions(${EXTRA_CFLAGS})

if(ANDROID)
    add_definitions(-D__ANDROID__)
    include_directories(/usr/src/SDL2_mixer/include)
    set(ANDROID_PATH_LIB -L/usr/src/SDL2_mixer/lib)
endif()

# Build include directories list conditionally
set(BGSOUND_INCLUDE_DIRS ${SDL2_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/core/include ${CMAKE_SOURCE_DIR}/core/bgdrtm ${CMAKE_SOURCE_DIR}/modules/libbgsound ${INCLUDE_DIRECTORIES})

# Add SDL_mixer include directories if found
if(SDLMIXER_INCLUDE_DIR)
    list(APPEND BGSOUND_INCLUDE_DIRS ${SDLMIXER_INCLUDE_DIR})
endif()
if(SDL2_MIXER_INCLUDE_DIR)
    list(APPEND BGSOUND_INCLUDE_DIRS ${SDL2_MIXER_INCLUDE_DIR})
endif()
if(SDL2_MIXER_INCLUDE_DIRS)
    list(APPEND BGSOUND_INCLUDE_DIRS ${SDL2_MIXER_INCLUDE_DIRS})
endif()

include_directories(${BGSOUND_INCLUDE_DIRS})

file(GLOB SOURCES_LIBBGSOUND
     "${CMAKE_SOURCE_DIR}/modules/libbgsound/*.c"
     )

add_library(bgsound ${LIBRARY_BUILD_TYPE} ${SOURCES_LIBBGSOUND})

target_link_libraries(bgsound ${SDL2_LIBRARY} ${SDL2_LIBRARIES} ${SDLMIXER_LIBRARY} ${SDL2_MIXER_LIBRARY} ${SDL2_MIXER_LIBRARIES} bgdrtm)
