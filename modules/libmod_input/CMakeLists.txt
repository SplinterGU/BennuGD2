project(mod_input)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/Modules/common_defs.cmake)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS} ../../core/include ../../core/bgdrtm ../../modules/libbggfx  ../../modules/libbginput ../../modules/libmod_input)

file(GLOB SOURCES_LIBMOD_INPUT
     "../../modules/libmod_input/*.c"
     )

add_library(mod_input ${LIB_TYPE} ${SOURCES_LIBMOD_INPUT})
target_link_libraries(mod_input ${SDL2_LIBRARY} ${SDL2_LIBRARIES} -L../../bin lbgdrtm bggfx bginput)
