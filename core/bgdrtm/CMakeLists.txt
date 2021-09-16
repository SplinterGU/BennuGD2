project(bgdrtm)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/Modules/common_defs.cmake)

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

add_definitions(-D__BGDRTM__ -DVERSION="2.0.0" ${extra_cflags})

include_directories(../../core/include ../../core/bgdrtm)

file(GLOB SOURCES_BGDRTM
    "../../core/bgdrtm/*.c"
    "../../core/common/*.c"
    )

add_library(bgdrtm ${LIB_TYPE} ${SOURCES_BGDRTM})
target_link_libraries(bgdrtm -lz ${extra_libs})
