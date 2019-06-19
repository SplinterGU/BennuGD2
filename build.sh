#!/bin/bash

show_help() {
    echo "usage:"
    echo "    $0 [windows|windows32|linux|linux32] [debug] [clean] [packages]"
    exit 1
}

BUILD_TYPE=Release

for i in "$@"
do
    case $i in
        windows)
            TARGET=x86_64-w64-mingw32
            CMAKE_EXTRA="-DCMAKE_TOOLCHAIN_FILE=../Toolchains/Toolchain-cross-mingw32-linux.cmake -DSDL2_INCLUDE_DIR=/usr/x86_64-w64-mingw32/include/SDL2"
            ;;

        linux)
            TARGET=linux-gnu
            ;;

        windows32)
            TARGET=i686-w64-mingw32
            CMAKE_EXTRA="-DBUILD_WIN32=ON -DCMAKE_TOOLCHAIN_FILE=../Toolchains/Toolchain-cross-mingw32-linux.cmake -DSDL2_INCLUDE_DIR=/usr/i686-w64-mingw32/include/SDL2 -DSDL2_LIBRARY=/usr/${TARGET}/bin/SDL2.dll -DSDL2_IMAGE_LIBRARY=/usr/${TARGET}/bin/SDL2_image.dll -DSDLMIXER_LIBRARY=/usr/${TARGET}/bin/SDL2_mixer.dll"
            ;;

        linux32)
            TARGET=i386-linux-gnu
            CMAKE_EXTRA="-DCMAKE_TOOLCHAIN_FILE=../Toolchains/linux_i686.toolchain.cmake -DSDL2_INCLUDE_DIR=/usr/include/SDL2 -DSDL2_LIBRARY=/usr/lib/${TARGET}/libSDL2-2.0.so.0 -DSDL2_IMAGE_LIBRARY=/usr/lib/${TARGET}/libSDL2_image-2.0.so.0 -DSDLMIXER_LIBRARY=/usr/lib/${TARGET}/libSDL2_mixer-2.0.so.0"
            ;;

        debug)
            BUILD_TYPE=RelWithDebInfo
            ;;

        clean)
            CLEAN=1
            ;;

        packages)
            echo "#### Building packages ####"
            mkdir -p packages 2>/dev/null
            rm -f packages/*
            rar a -ep1 packages/bgd2-x86_64-w64-mingw32-$(date +"%Y-%m-%d").rar cmake/build-x86_64-w64-mingw32/bin/* dependencies/x86_64-w64-mingw32/* WhatsNew.txt
            tar -zcvf packages/bgd2-linux-gnu-$(date +"%Y-%m-%d").tgz cmake/build-linux-gnu/bin/* WhatsNew.txt
            exit 0
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

mkdir -p cmake/build-$TARGET 2>/dev/null
mkdir -p bin/$TARGET 2>/dev/null

echo "### Building BennuGD ($TARGET) ###"

cd cmake/build-$TARGET
if [ "$CLEAN" == "1" ]
then
    rm CMakeCache.txt
fi
cmake .. $DEBUG -DCMAKE_BUILD_TYPE=$BUILD_TYPE $CMAKE_EXTRA
if [ "$CLEAN" == "1" ]
then
    make clean
fi
make
cd -

echo "### Build done! ###"

exit 0
