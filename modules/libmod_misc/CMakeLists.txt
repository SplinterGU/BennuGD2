project(mod_misc)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/Modules/common_defs.cmake)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS} ../../core/include ../../core/bgdrtm ../../modules/libmod_misc)

file(GLOB SOURCES_LIBMOD_MISC
     "../../modules/libmod_misc/*.c"
     )

add_library(mod_misc ${LIB_TYPE} ${SOURCES_LIBMOD_MISC})
target_link_libraries(mod_misc ${SDL2_LIBRARY} ${SDL2_LIBRARIES} -L../../bin bgdrtm)
