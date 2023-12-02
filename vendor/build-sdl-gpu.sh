#!/bin/bash

show_help() {
    echo "usage:"
    echo "    $0 [windows|windows32|linux|linux32|switch] [debug] [clean] [verbose]"
    exit 1
}

BUILD_TYPE=Release

for i in "$@"
do
    case $i in
        windows)
            TARGET=x86_64-w64-mingw32
            if [ "$MSYSTEM" != "MINGW64" ] && [ "$MSYSTEM" != "MINGW32" ]; then
                # linux
                CMAKE_EXTRA="-DCMAKE_TOOLCHAIN_FILE=../../cmake/Toolchains/Toolchain-cross-mingw32-linux.cmake -DSDL2_INCLUDE_DIR=/usr/x86_64-w64-mingw32/include/SDL2"
            fi
            ;;

        linux)
            TARGET=linux-gnu
            ;;

        windows32)
            TARGET=i686-w64-mingw32
            if [ "$MSYSTEM" != "MINGW64" ] && [ "$MSYSTEM" != "MINGW32" ]; then
                # linux
                CMAKE_EXTRA="-DBUILD_WIN32=ON -DCMAKE_TOOLCHAIN_FILE=../../cmake/Toolchains/Toolchain-cross-mingw32-linux.cmake -DSDL2_INCLUDE_DIR=/usr/i686-w64-mingw32/include/SDL2 -DSDL2_LIBRARY=/usr/${TARGET}/bin/SDL2.dll -DSDL2_IMAGE_LIBRARY=/usr/${TARGET}/bin/SDL2_image.dll -DSDLMIXER_LIBRARY=/usr/${TARGET}/bin/SDL2_mixer.dll"
            fi
            ;;

        linux32)
            TARGET=i386-linux-gnu
            CMAKE_EXTRA="-DCMAKE_TOOLCHAIN_FILE=../../cmake/Toolchains/linux_i686.toolchain.cmake -DSDL2_INCLUDE_DIR=/usr/include/SDL2 -DSDL2_LIBRARY=/usr/lib/${TARGET}/libSDL2-2.0.so.0 -DSDL2_IMAGE_LIBRARY=/usr/lib/${TARGET}/libSDL2_image-2.0.so.0 -DSDLMIXER_LIBRARY=/usr/lib/${TARGET}/libSDL2_mixer-2.0.so.0 -DOPENGL_gl_LIBRARY=/usr/lib/${TARGET}/libGL.so.1 -DOPENGL_glu_LIBRARY=/usr/lib/${TARGET}/libGLU.so.1"
            ;;

        switch)
            TARGET=aarch64-none-elf
            if [ "$MSYSTEM" != "MINGW64" ] && [ "$MSYSTEM" != "MINGW32" ]; then
                # linux
                CMAKE_EXTRA="-DCMAKE_TOOLCHAIN_FILE=$DEVKITPRO/cmake/Switch.cmake -DSWITCH=1 -DSDL2_INCLUDE_DIR=$DEVKITPRO/portlibs/switch/include/SDL2 -DSDL2_LIBRARY=$DEVKITPRO/portlibs/switch/lib/libSDL2.a"
            fi
            INCLUDE_DIRECTORIES="$DEVKITPRO/portlibs/switch/include;$DEVKITPRO/libnx/include;$DEVKITPRO/devkitA64/include"
            export INCLUDE_DIRECTORIES
            ;;

        debug)
            BUILD_TYPE=RelWithDebInfo
            ;;

        clean)
            CLEAN=1
            ;;

        verbose)
            VERBOSE="-DCMAKE_VERBOSE_MAKEFILE=ON"
            ;;

        *)
            # unknown option
            show_help
            ;;
    esac
done

if [ "$TARGET" == "" ]
then
    show_help
fi

export PKG_CONFIG_PATH
export TARGET

echo "### Building SDL_gpu ($TARGET) ###"

if [ "$CLEAN" == "1" ]
then
    rm -rf sdl-gpu/build/$TARGET
fi

mkdir -p sdl-gpu/build/$TARGET 2>/dev/null

cd sdl-gpu/build/$TARGET

#cmake ../.. $DEBUG -DCMAKE_BUILD_TYPE=$BUILD_TYPE $CMAKE_EXTRA $VERBOSE -DSDL_gpu_BUILD_TESTS=YES -DSDL_gpu_BUILD_VIDEO_TEST=YES -DSDL_gpu_BUILD_TOOLS=YES
#cmake ../.. $DEBUG -DCMAKE_BUILD_TYPE=$BUILD_TYPE $CMAKE_EXTRA $VERBOSE -DSDL_gpu_BUILD_TESTS=YES -DBUILD_TESTS=YES -DBUILD_VIDEO_TEST=YES -DBUILD_TOOLS=YES
cmake ../.. $DEBUG -DINCLUDE_DIRECTORIES="${INCLUDE_DIRECTORIES}" -DCMAKE_BUILD_TYPE=$BUILD_TYPE $CMAKE_EXTRA $VERBOSE -DSDL_gpu_BUILD_TESTS=NO -DBUILD_TESTS=NO -DBUILD_VIDEO_TEST=NO -DBUILD_TOOLS=NO -DBUILD_DEMOS=NO -DBUILD_STATIC=YES
if grep -q "CMAKE_GENERATOR:INTERNAL=Ninja" CMakeCache.txt; then
    ninja
elif grep -q "CMAKE_GENERATOR:INTERNAL=Unix Makefiles" CMakeCache.txt; then
    make -j
fi

if [ $? -eq 0 ]; then
    case $TARGET in
        x86_64-w64-mingw32)
            cp SDL_gpu-MINGW/bin/*.dll ../../../../dependencies/$TARGET
            ;;

        linux-gnu)
            cp SDL_gpu/lib/*.so ../../../../dependencies/$TARGET
            ;;

        i686-w64-mingw32)
            cp SDL_gpu-MINGW/bin/*.dll ../../../../dependencies/$TARGET
            ;;

        i386-linux-gnu)
            cp SDL_gpu/lib/*.so ../../../../dependencies/$TARGET
            cp SDL_gpu/lib/*.so SDL_gpu/lib/*.a /usr/lib/${TARGET}
            ;;

    esac
    
fi
cd -

echo "### Build done! ###"

exit 0

