#!/bin/bash

show_help() {
    echo "usage:"
    echo "    $0 [windows|linux] [debug] [clean] [packages]"
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
            rar a -ep1 packages/bgd2-x86_64-w64-mingw32-$(date +"%Y-%m-%d").rar cmake/build-x86_64-w64-mingw32/bin/* dependencies/x86_64-w64-mingw32/*
            tar -zcvf packages/bgd2-linux-gnu-$(date +"%Y-%m-%d").tgz cmake/build-linux-gnu/bin/*
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
