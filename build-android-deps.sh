#!/usr/bin/env bash

set -e

# CONFIGURACIÓN GENERAL
NDK="${ANDROID_NDK:-$HOME/Android/Sdk/ndk/27.0.12077973}"
TOOLCHAIN="$NDK/toolchains/llvm/prebuilt/linux-x86_64"
CMAKE_TOOLCHAIN="$NDK/build/cmake/android.toolchain.cmake"
API=21
BUILD_JOBS=$(nproc)
PREFIX_BASE="$(pwd)/vendor/android"

# FUNCIONES
download_and_extract() {
  local url=$1
  local dir=$2
  [ -d "$dir" ] || {
    curl -LO "$url"
    tar xf $2.tar.gz
  }
}

build_for_abi() {
  local ABI=$1
  local HOST=$2
  local PREFIX=$3

  local SYSROOT="$TOOLCHAIN/sysroot"
  local CC="$TOOLCHAIN/bin/${HOST}${API}-clang"
  local AR="$TOOLCHAIN/bin/llvm-ar"
  local RANLIB="$TOOLCHAIN/bin/llvm-ranlib"
  local CFLAGS="--sysroot=$SYSROOT"
  local LDFLAGS="-L$PREFIX/lib"

  echo "=== Compilando para $ABI ($HOST) ==="

  # SDL2
  cd SDL2-2.32.6
  make distclean 2>/dev/null || true
  mkdir -p build-$ABI && cd build-$ABI
  cmake .. \
    -DCMAKE_TOOLCHAIN_FILE=$CMAKE_TOOLCHAIN \
    -DANDROID_ABI=$ABI \
    -DANDROID_PLATFORM=android-$API \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=$PREFIX
  make -j$BUILD_JOBS
  make install
  cd ../..

  # SDL2_image
  cd SDL2_image-2.8.8
  make distclean 2>/dev/null || true
  mkdir -p build-$ABI && cd build-$ABI
  cmake .. \
    -DCMAKE_TOOLCHAIN_FILE=$CMAKE_TOOLCHAIN \
    -DANDROID_ABI=$ABI \
    -DANDROID_PLATFORM=android-$API \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=$PREFIX \
    -DSDL2IMAGE_SAMPLES=OFF \
    -DSDL2_DIR=$PREFIX \
    -DSDL2_LIBRARY=$PREFIX/lib/libSDL2.so \
    -DSDL2_INCLUDE_DIR=$PREFIX/include/SDL2
  make -j$BUILD_JOBS
  make install
  cd ../..

  # libogg
  cd libogg-1.3.5
  make distclean 2>/dev/null || true
  ./configure \
    --host=$HOST \
    --prefix=$PREFIX \
    --enable-shared \
    --disable-static \
    CC=$CC AR=$AR RANLIB=$RANLIB CFLAGS="$CFLAGS"
  make -j$BUILD_JOBS
  make install
  cd ..

  # libvorbis
  cd libvorbis-1.3.7
  make distclean 2>/dev/null || true
  unset CONFIGURE_LDFLAGS
  if [ "$ABI" = "x86" ]; then
    CONFIGURE_LDFLAGS="-lm"
  fi
  ./configure \
    --host=$HOST \
    --prefix=$PREFIX \
    --enable-shared \
    --disable-static \
    CC=$CC AR=$AR RANLIB=$RANLIB \
    CFLAGS="$CFLAGS -I$PREFIX/include" \
    LDFLAGS="$LDFLAGS $CONFIGURE_LDFLAGS"
  if [ "$ABI" = "x86" ]; then
    find . -type f -name Makefile -exec sed -i 's/ *-mno-ieee-fp//g' {} +
    sed -i 's,#define size_t,//#define size_t,g' config.h
  fi
  make -j$BUILD_JOBS
  make install
  cd ..

  # libtheora
  cd theora
  make distclean 2>/dev/null || true
  autoreconf -fi
  CONFIGURE_FLAGS="--host=$HOST \
  --prefix=$PREFIX \
  --enable-shared \
  --disable-static \
  --disable-examples \
  --disable-ld-version-script"
  # Desactivar ASM solo para armeabi-v7a
  if [ "$ABI" = "armeabi-v7a" ]; then
    CONFIGURE_FLAGS="$CONFIGURE_FLAGS --disable-asm"
  fi
  ./configure $CONFIGURE_FLAGS \
    CC="$CC" \
    AR="$AR" \
    RANLIB="$RANLIB" \
    CFLAGS="--sysroot=$SYSROOT -I$PREFIX/include" \
    LDFLAGS="-L$PREFIX/lib"

  sed -i 's/\(--version-info\)[[:space:]]*[0-9:]\+/\1 0:0:0/g' ltmain.sh
  sed -i '/-version-info/ s/-version-info[^\\]*//g' lib/Makefile

  make -j$BUILD_JOBS
  make install
  cd ..

  # SDL2_mixer
  cd SDL2_mixer-2.8.1
  make distclean 2>/dev/null || true
  mkdir -p build-$ABI && cd build-$ABI
  cmake .. \
    -DCMAKE_TOOLCHAIN_FILE=$CMAKE_TOOLCHAIN \
    -DANDROID_ABI=$ABI \
    -DANDROID_PLATFORM=android-$API \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=$PREFIX \
    -DSDL2MIXER_VENDORED=OFF \
    -DSDL2MIXER_SAMPLES=OFF \
    -DSDL2MIXER_OPUS=OFF \
    -DSDL2MIXER_FLAC=OFF \
    -DSDL2MIXER_MOD=OFF \
    -DSDL2MIXER_MP3=OFF \
    -DSDL2MIXER_MIDI=OFF \
    -DSDL2MIXER_OPUS=OFF \
    -DSDL2MIXER_WAVPACK=OFF \
    -DSDL2MIXER_GME=OFF \
    -DSDL2_DIR=$PREFIX \
    -DSDL2_LIBRARY=$PREFIX/lib/libSDL2.so \
    -DSDL2_INCLUDE_DIR=$PREFIX/include/SDL2 \
    -DOGG_LIBRARY=$PREFIX/lib/libogg.a \
    -DOGG_INCLUDE_DIR=$PREFIX/include \
    -DVORBIS_LIBRARY=$PREFIX/lib/libvorbis.a \
    -DVORBIS_INCLUDE_DIR=$PREFIX/include \
    -DVORBISFILE_LIBRARY=$PREFIX/lib/libvorbisfile.a
  make -j$BUILD_JOBS
  make install
  cd ../..

}

# DESCARGAS
mkdir -p vendor/android && cd vendor/android

download_and_extract https://libsdl.org/release/SDL2-2.32.6.tar.gz SDL2-2.32.6
download_and_extract https://www.libsdl.org/projects/SDL_image/release/SDL2_image-2.8.8.tar.gz SDL2_image-2.8.8
download_and_extract https://downloads.xiph.org/releases/ogg/libogg-1.3.5.tar.gz libogg-1.3.5
download_and_extract https://downloads.xiph.org/releases/vorbis/libvorbis-1.3.7.tar.gz libvorbis-1.3.7
download_and_extract https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-2.8.1.tar.gz SDL2_mixer-2.8.1

git clone https://github.com/xiph/theora.git || true

# COMPILACIÓN POR ABI
build_for_abi "armeabi-v7a"   "armv7a-linux-androideabi"  "$PREFIX_BASE/armv7a-linux-androideabi/armeabi-v7a"
build_for_abi "arm64-v8a"     "aarch64-linux-android"     "$PREFIX_BASE/aarch64-linux-android/arm64-v8a"
build_for_abi "x86"           "i686-linux-android"        "$PREFIX_BASE/i686-linux-android/x86"
build_for_abi "x86_64"        "x86_64-linux-android"      "$PREFIX_BASE/x86_64-linux-android/x86_64"
