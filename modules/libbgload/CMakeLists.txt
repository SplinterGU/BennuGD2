project(bgload)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "../../bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../../bin/")

set(CMAKE_MODULE_PATH ../../cmake/Modules)

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS} ../../core/include ../../core/bgdrtm ../../modules/libbgload)

set(SOURCES_LIBBGLOAD "../../modules/libbgload/bgload.c")

add_library(bgload SHARED ${SOURCES_LIBBGLOAD})
target_link_libraries(bgload ${SDL2_LIBRARY} ${SDL2_LIBRARIES} -L../../bin -lbgdrtm)
