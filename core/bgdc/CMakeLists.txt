project(bgdc)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../../bin/")

find_package(ZLIB REQUIRED)

option (USE_CRYPT "Use crypt functions." 0)
if (USE_CRYPT)
    find_package(OpenSSL REQUIRED)
    set(extra_cflags -DUSE_CRYPT=1)
    set(extra_libs -lcrypto)
endif ()

if(MINGW)
else ()
    set(extra_libs ${extra_libs} -ldl)
endif ()

if (LIBRARY_BUILD_TYPE STREQUAL "STATIC")
    file(GLOB MODULE_HEADERS "../../modules/*")

    find_package(SDL2 REQUIRED)
    find_package(SDL2_mixer REQUIRED)
    find_package(SDL2_image REQUIRED)
    if(USE_SDL2_GPU)
        find_package(SDL_GPU REQUIRED)
    endif()

    include_directories(${SDL2_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS} ${SDL_GPU_INCLUDE_DIR} ${SDLMIXER_INCLUDE_DIRS} ${SDL2_mixer_INCLUDE_DIRS} ${SDL2_image_INCLUDE_DIRS} ../bgdrtm ${MODULE_HEADERS})
endif()

add_definitions(-D__BGDC__ -DVERSION="2.0.0" ${extra_cflags})
add_definitions(${EXTRA_CFLAGS})

include_directories(../../core/include ../../core/bgdc)

file(GLOB SOURCES
     "../../core/bgdc/*.c"
     "../../core/common/*.c"
     )

add_executable(bgdc ${SOURCES})
target_link_libraries(bgdc -lz -lm ${extra_libs})
