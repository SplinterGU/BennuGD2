project(sdlhandler)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "../../bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../../bin/")

set(CMAKE_MODULE_PATH ../../cmake/Modules)

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS} ../../core/include ../../core/bgdrtm ../../modules/libsdlhandler)

file(GLOB SOURCES_LIBSDLHANDLER
     "../../modules/libsdlhandler/*.c"
     )

add_library(sdlhandler SHARED ${SOURCES_LIBSDLHANDLER})
target_link_libraries(sdlhandler ${SDL2_LIBRARY} ${SDL2_LIBRARIES} -L../../bin bgdrtm)
