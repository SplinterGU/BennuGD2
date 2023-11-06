#!/bin/bash

show_help() {
    echo "usage:"
    echo "    $0 [windows|windows32|linux|linux32|switch] [debug] [clean] [packages] [use_sdl2|use_sdl2_gpu] [verbose] [static] [one-job]"
    exit 1
}

BUILD_TYPE=Release
STATIC_ENABLED=0
LIBRARY_BUILD_TYPE=SHARED
USE_SDL2=0
USE_SDL2_GPU=1
EXTRA_CFLAGS=
MISC_FLAGS=
ONE_JOB=0

for i in "$@"
do
    case $i in
        use_sdl2)
            USE_SDL2=1
            USE_SDL2_GPU=0
            ;;

        use_sdl2_gpu)
            USE_SDL2=0
            USE_SDL2_GPU=1
            ;;

        windows)
            TARGET=x86_64-w64-mingw32
            COMPILER="-MINGW"
            SDL2GPUDIR="../../vendor/sdl-gpu/build/build-$ENV{TARGET}"
            if [ "$MSYSTEM" != "MINGW64" ] && [ "$MSYSTEM" != "MINGW32" ]; then
                # linux
                CMAKE_EXTRA="-DCMAKE_TOOLCHAIN_FILE=cmake/Toolchains/Toolchain-cross-mingw32-linux.cmake -DSDL2_INCLUDE_DIR=/usr/x86_64-w64-mingw32/include/SDL2"
            fi
            ;;

        switch)
            TARGET=aarch64-none-elf
            COMPILER=""
            SDL2GPUDIR="../../vendor/sdl-gpu/build/build-$ENV{TARGET}"
            STATIC_ENABLED=1 # force STATIC
            USE_SDL2=1
            USE_SDL2_GPU=0
            if [ "$MSYSTEM" != "MINGW64" ] && [ "$MSYSTEM" != "MINGW32" ]; then
                # linux
                CMAKE_EXTRA="-DCMAKE_TOOLCHAIN_FILE=$DEVKITPRO/cmake/Switch.cmake" #-DSDL2_INCLUDE_DIR=/usr/x86_64-w64-mingw32/include/SDL2
            fi
            INCLUDE_DIRECTORIES="$DEVKITPRO/portlibs/switch/include;$DEVKITPRO/libnx/include;$DEVKITPRO/devkitA64/include"
            export INCLUDE_DIRECTORIES
            ;;

        linux)
            TARGET=linux-gnu
            LIBVLC=1
            ;;

        windows32)
            TARGET=i686-w64-mingw32
            if [ "$MSYSTEM" != "MINGW64" ] && [ "$MSYSTEM" != "MINGW32" ]; then
                # linux
                CMAKE_EXTRA="-DBUILD_WIN32=ON -DCMAKE_TOOLCHAIN_FILE=cmake/Toolchains/Toolchain-cross-mingw32-linux.cmake -DSDL2_INCLUDE_DIR=/usr/i686-w64-mingw32/include/SDL2 -DSDL2_LIBRARY=/usr/${TARGET}/bin/SDL2.dll -DSDL2_IMAGE_LIBRARY=/usr/${TARGET}/bin/SDL2_image.dll -DSDLMIXER_LIBRARY=/usr/${TARGET}/bin/SDL2_mixer.dll"
            fi
            ;;

        linux32)
            TARGET=i386-linux-gnu
            if [ "$MSYSTEM" != "MINGW64" ] && [ "$MSYSTEM" != "MINGW32" ]; then
                # linux
                CMAKE_EXTRA="-DCMAKE_TOOLCHAIN_FILE=cmake/Toolchains/linux_i686.toolchain.cmake -DSDL2_INCLUDE_DIR=/usr/include/SDL2 -DSDL2_LIBRARY=/usr/lib/${TARGET}/libSDL2-2.0.so.0 -DSDL2_IMAGE_LIBRARY=/usr/lib/${TARGET}/libSDL2_image-2.0.so.0 -DSDLMIXER_LIBRARY=/usr/lib/${TARGET}/libSDL2_mixer-2.0.so.0"
            fi
            ;;

        static)
            STATIC_ENABLED=1
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

        packages)
            echo "#### Building packages ####"
            mkdir -p packages 2>/dev/null
            rm -f packages/*
            tar -zcvf packages/bgd2-linux-gnu-$(date +"%Y-%m-%d").tgz build/build-linux-gnu/bin/* dependencies/build-linux-gnu/* WhatsNew.txt --transform='s/[^\/]*\///g'
            tar -zcvf packages/bgd2-i386-linux-gnu-$(date +"%Y-%m-%d").tgz build/build-i386-linux-gnu/bin/* dependencies/build-i386-linux-gnu/* WhatsNew.txt --transform='s/[^\/]*\///g'
            rar a -ep1 packages/bgd2-i686-w64-mingw32-$(date +"%Y-%m-%d").rar build/build-i686-w64-mingw32/bin/* dependencies/i686-w64-mingw32/* WhatsNew.txt
            rar a -ep1 packages/bgd2-x86_64-w64-mingw32-$(date +"%Y-%m-%d").rar build/build-x86_64-w64-mingw32/bin/* dependencies/x86_64-w64-mingw32/* WhatsNew.txt
            exit 0
            ;;

        one-job)
            ONE_JOB=1
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

if [ "$LIBVLC" == "1" ]
then
    EXTRA_CFLAGS+=" -DLIBVLC_ENABLED=1"
    MISC_FLAGS+=" -DLIBVLC_ENABLED=1"
fi

if [ "$USE_SDL2" == "1" ]
then
    EXTRA_CFLAGS+=" -DUSE_SDL2=1"
else
    EXTRA_CFLAGS+=" -DUSE_SDL2_GPU=1"
    MISC_FLAGS+=" -DUSE_SDL2_GPU=1"
    export USE_SDL2_GPU
fi

if [ "$STATIC_ENABLED" == "1" ]
then
    EXTRA_CFLAGS+=" -D__STATIC__"
    LIBRARY_BUILD_TYPE=STATIC
fi

export PKG_CONFIG_PATH
export TARGET
export SDL2GPUDIR
export COMPILER
export LIBRARY_BUILD_TYPE

if [ "$CLEAN" == "1" ]
then
    echo "### Cleaning previous build ($TARGET) ###"
    rm -rf build/build-$TARGET
fi

echo "### Building BennuGD ($TARGET) ###"
mkdir -p build/build-$TARGET 2>/dev/null
cd build/build-$TARGET
cmake ../.. $DEBUG ${CMAKE_CXX_COMPILER} -DINCLUDE_DIRECTORIES="${INCLUDE_DIRECTORIES}" -DCMAKE_BUILD_TYPE=$BUILD_TYPE $CMAKE_EXTRA $VERBOSE -DEXTRA_CFLAGS="$EXTRA_CFLAGS" $MISC_FLAGS -DLIBRARY_BUILD_TYPE=$LIBRARY_BUILD_TYPE
if grep -q "CMAKE_GENERATOR:INTERNAL=Ninja" CMakeCache.txt; then
    ninja
elif grep -q "CMAKE_GENERATOR:INTERNAL=Unix Makefiles" CMakeCache.txt; then
    if [ $ONE_JOB -eq 0 ]; then
        make -j
    else
        make
    fi
fi
cd -

echo "### Build done! ###"

exit 0
