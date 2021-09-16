project(bgdi)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../../bin/")

find_package(ZLIB REQUIRED)

add_definitions(-D__BGDI__ -DVERSION="2.0.0")

include_directories(../../core/include ../../core/bgdi ../../core/bgdrtm)

file(GLOB SOURCES
     "../../core/bgdi/*.c"
     )

link_directories(../build/bin)

add_executable(bgdi ${SOURCES})
target_link_libraries(bgdi -L../../bin -lbgdrtm -lm)
