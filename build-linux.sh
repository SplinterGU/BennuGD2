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

TARGET=linux-gnu

export PKG_CONFIG_PATH
export TARGET

mkdir -p cmake/build-$TARGET 2>/dev/null
mkdir -p bin/$TARGET 2>/dev/null

echo "### Building BennuGD ###"

cd cmake/build-$TARGET
cmake .. $DEBUG
make $*
cd -

echo "### Build done! ###"

exit 0
