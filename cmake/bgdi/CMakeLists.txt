project(bgdi)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/")

find_package(ZLIB REQUIRED)
find_package(OpenSSL REQUIRED)

add_definitions(-D__BGDI__ -DVERSION="2.0.0")

include_directories(../../core/include ../../core/bgdi/include ../../core/bgdrtm/include)

file(GLOB SOURCES
     "../../core/bgdi/src/*.c"
     )

link_directories(../build/bin)

add_executable(bgdi ${SOURCES})
target_link_libraries(bgdi -lbgdrtm -lm)
