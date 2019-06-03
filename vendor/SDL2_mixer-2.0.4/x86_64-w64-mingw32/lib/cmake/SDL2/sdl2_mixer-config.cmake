# sdl2 cmake project-config input for ./configure scripts

set(prefix "/usr/x86_64-w64-mingw32")
set(exec_prefix "${prefix}")
set(libdir "${exec_prefix}/lib")
set(SDL2_mixer_PREFIX "/usr/x86_64-w64-mingw32")
set(SDL2_mixer_EXEC_PREFIX "/usr/x86_64-w64-mingw32")
set(SDL2_mixer_LIBDIR "${exec_prefix}/lib")
set(SDL2_mixer_INCLUDE_DIRS "${prefix}/include/SDL2")
set(SDL2_mixer_LIBRARIES "-L${SDL2_mixer_LIBDIR}  -lSDL2_mixer")
string(STRIP "${SDL2_mixer_LIBRARIES}" SDL2_mixer_LIBRARIES)
