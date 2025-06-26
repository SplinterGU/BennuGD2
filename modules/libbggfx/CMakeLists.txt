project(bggfx)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/${TARGET}/bin/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/${TARGET}/bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/${TARGET}/bin/")

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)

find_package(SDL2 REQUIRED)
if(USE_SDL2_GPU)
    find_package(SDL_GPU REQUIRED)
endif()

add_definitions(-D__LIBBGFGX ${EXTRA_CFLAGS})

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/core/include ${CMAKE_SOURCE_DIR}/core/bgdrtm ${CMAKE_SOURCE_DIR}/modules/libsdlhandler ${CMAKE_SOURCE_DIR}/modules/libbggfx ${SDL_GPU_INCLUDE_DIR} ${INCLUDE_DIRECTORIES} ${CMAKE_SOURCE_DIR}/vendor/theoraplay)

file(GLOB SOURCES_LIBBGGFX
     "${CMAKE_SOURCE_DIR}/modules/libbggfx/*.c"
     "${CMAKE_SOURCE_DIR}/vendor/theoraplay/theoraplay.c"
     )

if(MINGW)
    set(OGL_LIB -lopengl32)
endif()

if(APPLE)
    set(OGL_LIB -L$ENV{SDKROOT}/System/Library/Frameworks/OpenGL.framework/Libraries/ -lGL)
endif()

if(ANDROID)
    add_definitions(-D__ANDROID__ -DUSE_GLES2)
    include_directories(${PREFIX_JNI}/include)
    set(ANDROID_PATH_LIB -L${PREFIX_JNI}/lib)
    set(OGL_LIB GLESv1_CM GLESv2 OpenSLES log android)
endif()

add_library(bggfx ${LIBRARY_BUILD_TYPE} ${SOURCES_LIBBGGFX})

target_link_libraries(bggfx ${SDL2_LIBRARY} ${SDL2_LIBRARIES} ${SDL_GPU_LIBRARY} -L${CMAKE_SOURCE_DIR}/bin bgdrtm ${OGL_LIB} ${STDLIBSFLAGS} ${ANDROID_PATH_LIB} ogg vorbis theoradec sdlhandler)
