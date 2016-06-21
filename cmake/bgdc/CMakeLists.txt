project(bgdc)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/")

find_package(ZLIB REQUIRED)
find_package(OpenSSL REQUIRED)

add_definitions(-D__BGDC__ -DVERSION="2.0.0")

include_directories(../../core/include ../../core/bgdc/include)

file(GLOB SOURCES
     "../../core/bgdc/src/*.c"
     "../../core/common/*.c"
     )

add_executable(bgdc ${SOURCES})
target_link_libraries(bgdc -lz -lcrypto -ldl)
