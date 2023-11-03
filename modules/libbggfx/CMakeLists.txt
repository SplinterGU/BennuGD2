project(bggfx)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "../../bin/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "../../bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../../bin/")

set(CMAKE_MODULE_PATH ../../cmake/Modules)

find_package(SDL2 REQUIRED)
if(USE_SDL2_GPU)
    find_package(SDL_GPU REQUIRED)
endif()

add_definitions(-D__LIBBGFGX ${EXTRA_CFLAGS})

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS} ../../core/include ../../core/bgdrtm ../../modules/libbggfx ${SDL_GPU_INCLUDE_DIR})

file(GLOB SOURCES_LIBBGGFX
     "../../modules/libbggfx/*.c"
     )

if(MINGW)
    set(OGL_LIB -lopengl32)
endif()

if(LIBVLC_ENABLED)
    set(VLC_LIB -lvlc)
endif()

add_library(bggfx ${LIBRARY_BUILD_TYPE} ${SOURCES_LIBBGGFX})

target_link_libraries(bggfx ${SDL2_LIBRARY} ${SDL2_LIBRARIES} ${SDL_GPU_LIBRARY} -L../../bin bgdrtm ${OGL_LIB} ${VLC_LIB})
