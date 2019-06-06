project(bgdc)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/")

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

add_definitions(-D__BGDC__ -DVERSION="2.0.0" ${extra_cflags})

include_directories(../../core/include ../../core/bgdc)

file(GLOB SOURCES
     "../../core/bgdc/*.c"
     "../../core/common/*.c"
     )

add_executable(bgdc ${SOURCES})
target_link_libraries(bgdc -lz ${extra_libs})
