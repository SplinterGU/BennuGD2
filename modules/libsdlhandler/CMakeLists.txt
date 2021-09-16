project(sdlhandler)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/Modules/common_defs.cmake)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS} ../../core/include ../../core/bgdrtm ../../modules/libsdlhandler)

file(GLOB SOURCES_LIBSDLHANDLER
     "../../modules/libsdlhandler/*.c"
     )

add_library(sdlhandler ${LIB_TYPE} ${SOURCES_LIBSDLHANDLER})
target_link_libraries(sdlhandler ${SDL2_LIBRARY} ${SDL2_LIBRARIES} -L../../bin bgdrtm)
