project(moddesc)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/${TARGET}/bin/")

if(LINUX)
    set(extra_libs ${extra_libs} -ldl)
endif ()

add_definitions(-DVERSION="2.0.0")
add_definitions(${EXTRA_CFLAGS})

if (LIBRARY_BUILD_TYPE STREQUAL "STATIC")
    file(GLOB MODULE_HEADERS "${CMAKE_SOURCE_DIR}/modules/*")

    find_package(SDL2 REQUIRED)
    find_package(SDL2_mixer REQUIRED)
    find_package(SDL2_image REQUIRED)
    if(USE_SDL2_GPU)
        find_package(SDL_GPU REQUIRED)
    endif()

    include_directories(${SDL2_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS} ${SDL_GPU_INCLUDE_DIR} ${SDLMIXER_INCLUDE_DIRS} ${SDL2_mixer_INCLUDE_DIRS}  ${SDL2_image_INCLUDE_DIRS} ${MODULE_HEADERS})

    foreach(module ${MODULE_HEADERS})
        get_filename_component(lib_name ${module} NAME)

        string(REGEX REPLACE "^lib" "" lib_name ${lib_name})

        list(APPEND EXTRA_LIBS "${lib_name}")
    endforeach()
endif()

include_directories(${CMAKE_SOURCE_DIR}/core/include ${CMAKE_SOURCE_DIR}/core/bgdrtm ${CMAKE_SOURCE_DIR}/tools/moddesc ${INCLUDE_DIRECTORIES})

file(GLOB SOURCES
     "${CMAKE_SOURCE_DIR}/tools/moddesc/*.c"
     )

add_executable(moddesc ${SOURCES})

target_link_libraries(moddesc -L${CMAKE_SOURCE_DIR}/bin bgdrtm ${EXTRA_LIBS} -lm ${extra_libs})