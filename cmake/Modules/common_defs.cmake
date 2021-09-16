add_definitions($ENV{EXTRA_CFLAGS})

if(__STATIC__)
	set(LIB_TYPE SHARED)
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "../../bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../../bin/")
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/Modules ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

#Common requirements
find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(ZLIB REQUIRED)
find_package(PNG REQUIRED)
find_package(SDL2_image REQUIRED)

#Optional libraries
if(USE_SDL2_GPU)
	find_package(SDL_gpu REQUIRED)
endif()
if(USE_VLC)
	find_package(VLC REQUIRED)
endif()
if(USE_STEAMWORKS)
	find_package(STEAMWORKS REQUIRED)
endif()