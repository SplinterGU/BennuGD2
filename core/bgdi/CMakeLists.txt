project(bgdi)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../../bin/")

find_package(ZLIB REQUIRED)

if (LIBRARY_BUILD_TYPE STREQUAL "STATIC")
    file(GLOB MODULE_HEADERS "../../modules/*")

    find_package(SDL2 REQUIRED)
    find_package(SDL2_mixer REQUIRED)
    find_package(SDL2_image REQUIRED)
    if(USE_SDL2_GPU)
        find_package(SDL_GPU REQUIRED)
    endif()

    include_directories(${SDL2_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS} ${SDL_GPU_INCLUDE_DIR} ${SDLMIXER_INCLUDE_DIRS} ${SDL2_mixer_INCLUDE_DIRS} ../bgdrtm ${MODULE_HEADERS})

    foreach(module ${MODULE_HEADERS})
        get_filename_component(lib_name ${module} NAME)

        string(REGEX REPLACE "^lib" "" lib_name ${lib_name})

        list(APPEND EXTRA_LIBS "${lib_name}")
    endforeach()

    if(MINGW)
        set(OGL_LIB -lopengl32)
    endif()

    if(LINUX AND NOT PS3_PPU)
        set(OGL_LIB -lGL)
    endif()

    if(NINTENDO_SWITCH)
        set(OGL_LIB -lglad -lEGL -lGLESv2 -lglapi -ldrm_nouveau -lnx -lm)
        set(OTHER_LIBS -lpng -ljpeg -lwebp -lmpg123 -lmodplug -lopusfile -lopus -lvorbisfile -lvorbis -logg)
    endif()

    if(PS3_PPU)
        set(OTHER_LIBS -L/usr/local/ps3dev/portlibs/ppu/lib -L/usr/local/ps3dev/spu/spu/lib -L/usr/local/ps3dev/ppu/powerpc64-ps3-elf/lib -L/usr/local/ps3dev/portlibs/ppu/lib -L/usr/local/ps3dev/ppu/lib -lm -lgcm_sys -lrsx -lsysutil -lrt -llv2 -lio -laudio -lpng -lpng14 -ltiff -ljpeg)
    endif()

    if(LIBVLC_ENABLED)
        set(VLC_LIB -lvlc)
    endif()
endif()

if(NOT PS3_PPU)
    set(OTHER_LIBS ${OTHER_LIBS} -lm)
endif()

add_definitions(-D__BGDI__ -DVERSION="2.0.0")
add_definitions($ENV{EXTRA_CFLAGS})
add_definitions(${SDL2_CFLAGS})

include_directories(../../core/include ../../core/bgdi ../../core/bgdrtm ${INCLUDE_DIRECTORIES})

if(NINTENDO_SWITCH)
file(GLOB SOURCES
     "../../core/bgdi/*.cpp"
     "../../core/bgdi/*.c"
     )
else()
file(GLOB SOURCES
     "../../core/bgdi/*.c"
     )
endif()

if(MINGW)
   set(SOURCES ${SOURCES} bgdi.rc)
endif()

add_executable(bgdi ${SOURCES})

target_link_libraries(bgdi -L../../bin bgdrtm ${EXTRA_LIBS} ${SDL2_LIBS} ${SDL2_LIBRARY} ${SDL2_LIBRARIES} ${SDLMIXER_LIBRARY} ${SDL2_mixer_LIBRARIES} ${SDL2_IMAGE_LIBRARY} ${SDL2_image_LIBRARIES} ${SDL_GPU_LIBRARY} ${OGL_LIB} ${VLC_LIB} ${OTHER_LIBS})
