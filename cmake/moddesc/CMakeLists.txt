project(moddesc)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/")

add_definitions(-DVERSION="2.0.0")

include_directories(../../core/include ../../tools/moddesc)

file(GLOB SOURCES
     "../../tools/moddesc/*.c"
     )

add_executable(moddesc ${SOURCES})
target_link_libraries(moddesc -ldl)
