project(bgdrtm)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "../bin/")

find_package(ZLIB REQUIRED)
find_package(OpenSSL REQUIRED)

add_definitions(-D__BGDRTM__ -DVERSION="2.0.0" -DTARGET_LINUX)

include_directories(../../core/include ../../core/bgdrtm/include)

file(GLOB SOURCES_BGDRTM
    "../../core/bgdrtm/src/*.c"
    "../../core/common/*.c"
    )

add_library(bgdrtm SHARED ${SOURCES_BGDRTM})
target_link_libraries(bgdrtm -lz -lcrypto -ldl)
