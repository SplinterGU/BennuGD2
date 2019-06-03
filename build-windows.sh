#!/bin/sh

for i in "$@"
do
    case $i in
        debug)
            DEBUG="-DCMAKE_BUILD_TYPE=Debug"
            ;;
        clean)
            CLEAN=1
            ;;
        *)
            # unknown option
            ;;
    esac
done

TARGET=x86_64-w64-mingw32

export PKG_CONFIG_PATH
export TARGET

mkdir -p cmake/build-$TARGET 2>/dev/null
mkdir -p bin/$TARGET 2>/dev/null

echo "### Building BennuGD ###"

cd cmake/build-$TARGET
cmake .. -DCMAKE_TOOLCHAIN_FILE=../Toolchains/Toolchain-cross-mingw32-linux.cmake -DSDL2_INCLUDE_DIR=/usr/x86_64-w64-mingw32/include/SDL2 -DSDL2_image_DIR=/usr/x86_64-w64-mingw32/lib/cmake/SDL2 -DSDL2_mixer_DIR=/usr/x86_64-w64-mingw32/lib/cmake/SDL2 $DEBUG
make $*
cd -

echo "### Build done! ###"

exit 0
