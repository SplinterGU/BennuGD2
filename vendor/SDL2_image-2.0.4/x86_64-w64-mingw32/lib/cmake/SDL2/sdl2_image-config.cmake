# sdl2 cmake project-config input for ./configure scripts

set(prefix "/usr/x86_64-w64-mingw32")
set(exec_prefix "${prefix}")
set(libdir "${exec_prefix}/lib")
set(SDL2_image_PREFIX "/usr/x86_64-w64-mingw32")
set(SDL2_image_EXEC_PREFIX "/usr/x86_64-w64-mingw32")
set(SDL2_image_LIBDIR "${exec_prefix}/lib")
set(SDL2_image_INCLUDE_DIRS "${prefix}/include/SDL2")
set(SDL2_image_LIBRARIES "-L${SDL2_image_LIBDIR}  -lSDL2_image")
string(STRIP "${SDL2_image_LIBRARIES}" SDL2_image_LIBRARIES)
