FIND_PACKAGE(Threads)

find_path(SDL_GPU_INCLUDE_DIR
    SDL_gpu.h
    PATHS ../../vendor/sdl-gpu/build/$ENV{TARGET} ../../vendor/sdl-gpu/build/$ENV{TARGET}/SDL_gpu ../../vendor/sdl-gpu/build/$ENV{TARGET}/SDL_gpu/include ../../vendor/sdl-gpu/build/$ENV{TARGET}/SDL_gpu$ENV{COMPILER}/include ../../vendor/sdl-gpu/build/$ENV{TARGET}/SDL_gpu$ENV{COMPILER} /usr /usr/local /opt /opt/local /Library/Frameworks ~/Library/Frameworks
    HINTS $ENV{SDL2GPUDIR}
	PATH_SUFFIXES include include/SDL2 include/SDL_gpu SDL2 SDL_gpu
	NO_CMAKE_FIND_ROOT_PATH NO_CMAKE_SYSTEM_PATH)

find_library(SDL_GPU_LIBRARY
    NAMES SDL2_gpu libSDL2_gpu.dll.a libSDL2_gpu_s.a libSDL2_gpu.dll
    HINTS $ENV{SDL2GPUDIR}
    PATHS ../../vendor/sdl-gpu/build/$ENV{TARGET} ../../vendor/sdl-gpu/build/$ENV{TARGET}/SDL_gpu ../../vendor/sdl-gpu/build/$ENV{TARGET}/SDL_gpu/lib ../../vendor/sdl-gpu/build/$ENV{TARGET}/SDL_gpu/bin ../../vendor/sdl-gpu/build/$ENV{TARGET}/SDL_gpu$ENV{COMPILER} ../../vendor/sdl-gpu/build/$ENV{TARGET}/SDL_gpu$ENV{COMPILER}/lib ../../vendor/sdl-gpu/build/$ENV{TARGET}/SDL_gpu$ENV{COMPILER}/bin /usr/$ENV{TARGET} /usr/local/lib
    PATH_SUFFIXES lib lib64 bin
    NO_CMAKE_FIND_ROOT_PATH NO_CMAKE_SYSTEM_PATH)

SET(SDL_GPU_LIBRARY ${SDL_GPU_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SDL_GPU DEFAULT_MSG SDL_GPU_LIBRARY SDL_GPU_INCLUDE_DIR)

#set to string so it shows up in gui
set(SDL_GPU_LIBRARY ${SDL_GPU_LIBRARY} CACHE FILEPATH "Where SDL_gpu library is located")
set(SDL_GPU_INCLUDE_DIR ${SDL_GPU_INCLUDE_DIR} CACHE PATH "Where SDL_gpu.h is located")
