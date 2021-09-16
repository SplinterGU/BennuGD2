project(bggfx)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "../../bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../../bin/")

set(CMAKE_MODULE_PATH ../../cmake/Modules)

find_package(SDL2 REQUIRED)
find_package(SDL_GPU REQUIRED)

add_definitions(-D__LIBBGFGX $ENV{EXTRA_CFLAGS})

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS} ../../core/include ../../core/bgdrtm ../../modules/libbggfx ${SDL_GPU_INCLUDE_DIR})

file(GLOB SOURCES_LIBBGGFX
     "../../modules/libbggfx/*.c"
     )

add_library(bggfx SHARED ${SOURCES_LIBBGGFX})
if(MINGW)
set(OGL_LIB -lopengl32)
endif()
target_link_libraries(bggfx ${SDL2_LIBRARY} ${SDL2_LIBRARIES} ${SDL_GPU_LIBRARY} -L../../bin -lbgdrtm ${OGL_LIB} -lvlc)
