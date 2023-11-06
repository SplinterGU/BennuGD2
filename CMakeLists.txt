cmake_minimum_required(VERSION 3.5)
project(bennugd2)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

# core
add_subdirectory(core/bgdc)
add_subdirectory(core/bgdrtm)
# tools
add_subdirectory(tools/moddesc)
add_subdirectory(core/bgdi)

# modules
set(MODULES_DIR modules)
file(GLOB MODULES_LIST "${MODULES_DIR}/lib*")

foreach(MODULE ${MODULES_LIST})
    if(IS_DIRECTORY "${MODULE}")
        add_subdirectory(${MODULE})
    endif()
endforeach()

