project(bgdi)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/Modules/common_defs.cmake)

add_definitions(-D__BGDI__ -DVERSION="2.0.0")

include_directories(../../core/include ../../core/bgdi ../../core/bgdrtm)

file(GLOB SOURCES
     "../../core/bgdi/*.c"
     )

link_directories(../build/bin)

add_executable(bgdi ${SOURCES})
target_link_libraries(bgdi -L../../bin bgdrtm -lm)
