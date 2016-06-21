#!/bin/sh

TARGET=linux-gnu

export PKG_CONFIG_PATH
export TARGET

mkdir -p cmake/build 2>/dev/null
mkdir -p bin/$TARGET 2>/dev/null

echo "### Building BennuGD ###"

cd cmake/build
cmake ..
make $*
cd -

echo "### Copying files to bin folder ###"

cp cmake/build/bin/* bin/$TARGET

echo "### Build done! ###"

exit 0
