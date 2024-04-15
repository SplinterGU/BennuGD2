project(bgdrtm)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "../../bin/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "../../bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../../bin/")

find_package(ZLIB REQUIRED)

option (USE_CRYPT "Use crypt functions." 0)
if (USE_CRYPT)
    find_package(OpenSSL REQUIRED)
    set(extra_cflags -DUSE_CRYPT=1)
    set(extra_libs -lcrypto)
endif()

if(LINUX AND NOT PS3_PPU)
    set(extra_libs ${extra_libs} -ldl)
endif()

if(NOT PS3_PPU)
    set(extra_libs ${extra_libs} -lm)
endif()

add_definitions(-D__BGDRTM__ -DVERSION="2.0.0" ${extra_cflags})
add_definitions(${EXTRA_CFLAGS})

include_directories(../../core/include ../../core/bgdrtm ${INCLUDE_DIRECTORIES} ${ZLIB_INCLUDE_DIR})

file(GLOB SOURCES_BGDRTM
    "../../core/bgdrtm/*.c"
    "../../core/common/*.c"
    )

add_library(bgdrtm ${LIBRARY_BUILD_TYPE} ${SOURCES_BGDRTM})

target_link_libraries(bgdrtm -lz ${extra_libs})
