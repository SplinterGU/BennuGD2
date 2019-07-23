FIND_PACKAGE(Threads)

find_path(SDL_GPU_INCLUDE_DIR
        SDL_gpu.h
    PATHS /usr/ /usr/local/ /opt/ /opt/local/ /Library/Frameworks/ ~/Library/Frameworks/ ../../vendor/sdl-gpu/build/build-$ENV{TARGET}/
    HINTS $ENV{SDL2GPUDIR}
	PATH_SUFFIXES include/ include/SDL2/ include/SDL_gpu SDL2 SDL_gpu
	NO_CMAKE_FIND_ROOT_PATH)

find_library(SDL_GPU_LIBRARY
    NAMES SDL2_gpu libSDL2_gpu.dll.a
    HINTS $ENV{SDL2GPUDIR}
    PATHS /usr/local/lib/ /usr/$ENV{TARGET}/ ../../vendor/sdl-gpu/build/build-$ENV{TARGET}/SDL_gpu/
    PATH_SUFFIXES lib/ lib64/ bin/
    NO_CMAKE_FIND_ROOT_PATH)

SET(SDL_GPU_LIBRARY ${SDL_GPU_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SDL_GPU DEFAULT_MSG SDL_GPU_LIBRARY SDL_GPU_INCLUDE_DIR)

#set to string so it shows up in gui
set(SDL_GPU_LIBRARY ${SDL_GPU_LIBRARY} CACHE FILEPATH "Where SDL_gpu library is located")
set(SDL_GPU_INCLUDE_DIR ${SDL_GPU_INCLUDE_DIR} CACHE PATH "Where SDL_gpu.h is located")
