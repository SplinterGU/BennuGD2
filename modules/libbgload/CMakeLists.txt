project(bgload)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/Modules/common_defs.cmake)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS} ../../core/include ../../core/bgdrtm ../../modules/libbgload)

set(SOURCES_LIBBGLOAD "../../modules/libbgload/bgload.c")

add_library(bgload ${LIB_TYPE} ${SOURCES_LIBBGLOAD})
target_link_libraries(bgload ${SDL2_LIBRARY} ${SDL2_LIBRARIES} -L../../bin bgdrtm)
