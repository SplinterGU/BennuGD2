cmake_minimum_required(VERSION 3.5)
project(bennugd2)

option(BUILD_TARGET "Select the build target: compiler, interpreter, tools, or all" "all")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

if(${PS3_PPU})
    set(PS3_PPU 1)
    add_definitions(-DPS3_PPU=1)
    include_directories(${CMAKE_INCLUDE_DIRECTORIES} $ENV{PS3DEV}/ppu/include/ $ENV{PS3DEV}/portlibs/ppu/include)
endif()

if(NINTENDO_SWITCH OR ANDROID)
    add_definitions(-DUSE_OPENDIR)
endif()

# strip
if(NOT CMAKE_BUILD_TYPE MATCHES "^(RelWithDebInfo|Debug)$")
    if (CMAKE_COMPILER_IS_GNUCC)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s")
    endif()
endif()

# core
add_subdirectory(core/bgdrtm)

string(FIND ${BUILD_TARGET} "compiler" BUILD_COMPILER)
string(FIND ${BUILD_TARGET} "interpreter" BUILD_INTERPRETER)
string(FIND ${BUILD_TARGET} "tools" BUILD_TOOLS)
string(FIND ${BUILD_TARGET} "all" BUILD_ALL)

if (NOT BUILD_COMPILER EQUAL -1 OR NOT BUILD_ALL EQUAL -1 OR "${BUILD_TARGET}" STREQUAL "OFF")
    add_subdirectory(core/bgdc)
endif ()

if (NOT BUILD_INTERPRETER EQUAL -1 OR NOT BUILD_ALL EQUAL -1 OR "${BUILD_TARGET}" STREQUAL "OFF")
    add_subdirectory(core/bgdi)
endif ()

# tools
if (NOT BUILD_TOOLS EQUAL -1 OR NOT BUILD_ALL EQUAL -1 OR "${BUILD_TARGET}" STREQUAL "OFF")
    add_subdirectory(tools/moddesc)
endif ()

# modules
set(MODULES_DIR modules)
file(GLOB MODULES_LIST "${MODULES_DIR}/lib*")

foreach(MODULE ${MODULES_LIST})
    if(IS_DIRECTORY "${MODULE}")
        add_subdirectory(${MODULE})
    endif()
endforeach()

