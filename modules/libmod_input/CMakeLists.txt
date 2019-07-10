project(mod_input)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "../../bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../../bin/")

set(CMAKE_MODULE_PATH ../../cmake/Modules)

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS} ../../core/include ../../core/bgdrtm ../../modules/libbggfx  ../../modules/libbginput ../../modules/libmod_input)

file(GLOB SOURCES_LIBMOD_INPUT
     "../../modules/libmod_input/*.c"
     )

add_library(mod_input SHARED ${SOURCES_LIBMOD_INPUT})
target_link_libraries(mod_input ${SDL2_LIBRARY} ${SDL2_LIBRARIES} -L../../bin -lbgdrtm -lbggfx -lbginput)
