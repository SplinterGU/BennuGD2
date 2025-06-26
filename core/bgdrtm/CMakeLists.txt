project(bgdrtm)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/${TARGET}/bin/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/${TARGET}/bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/${TARGET}/bin/")

find_package(ZLIB REQUIRED)

option (USE_CRYPT "Use crypt functions." 0)
if (USE_CRYPT)
    find_package(OpenSSL REQUIRED)
    set(extra_cflags -DUSE_CRYPT=1)
    set(APPEND extra_libs crypto)
endif()

if(LINUX AND NOT PS3_PPU)
    set(APPEND extra_libs dl)
endif()

if(NOT PS3_PPU)
    set(APPEND extra_libs m)
endif()

if(ANDROID)
    list(APPEND extra_libs log)

    find_package(SDL2 REQUIRED)

    include_directories(${SDL2_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS})

    add_definitions(-D__ANDROID__ -DUSE_GLES2)
    include_directories(${CMAKE_SOURCE_DIR}/vendor/android/include)
    set(ANDROID_PATH_LIB -L${CMAKE_SOURCE_DIR}/vendor/android/lib)
    set(OGL_LIB GLESv1_CM GLESv2 OpenSLES log android)
endif()

add_definitions(-D__BGDRTM__ -DVERSION="2.0.0" ${extra_cflags})
add_definitions(${EXTRA_CFLAGS})

include_directories(${CMAKE_SOURCE_DIR}/core/include ${CMAKE_SOURCE_DIR}/core/bgdrtm ${INCLUDE_DIRECTORIES} ${ZLIB_INCLUDE_DIR})

file(GLOB SOURCES_BGDRTM
    "${CMAKE_SOURCE_DIR}/core/bgdrtm/*.c"
    "${CMAKE_SOURCE_DIR}/core/common/*.c"
    )

add_library(bgdrtm ${LIBRARY_BUILD_TYPE} ${SOURCES_BGDRTM})

if(ANDROID)
    target_link_libraries(bgdrtm -lz ${extra_libs} ${SDL2_LIBRARY} ${SDL2_LIBRARIES} -L${CMAKE_SOURCE_DIR}/bin ${OGL_LIB} ${STDLIBSFLAGS} ${ANDROID_PATH_LIB})
else()
    target_link_libraries(bgdrtm -lz ${extra_libs})
endif()

