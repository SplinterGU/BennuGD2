#!/bin/bash

show_help() {
    echo "usage:"
    echo "    $0 [windows|windows32|linux|linux32|switch] [debug] [clean] [verbose] [one-job]"
    exit 1
}


build_target() {
    BUILD_TYPE=Release
    ONE_JOB=0

    for i in "$@"
    do
        case $i in
            windows)
                TARGET=x86_64-w64-mingw32
                if [ "$MSYSTEM" = "" ]; then
                    # linux
                    CMAKE_EXTRA="-DCMAKE_TOOLCHAIN_FILE=../../cmake/Toolchains/Toolchain-cross-mingw32-linux.cmake -DSDL2_INCLUDE_DIR=/usr/${TARGET}/include/SDL2"
                fi
                ;;

            linux)
                TARGET=linux-gnu
                ;;

            windows32)
                TARGET=i686-w64-mingw32
                if [ "$MSYSTEM" = "" ]; then
                    # linux
                    CMAKE_EXTRA="-DBUILD_WIN32=ON -DCMAKE_TOOLCHAIN_FILE=../../cmake/Toolchains/Toolchain-cross-mingw32-linux.cmake -DSDL2_INCLUDE_DIR=/usr/${TARGET}/include/SDL2 -DSDL2_LIBRARY=/usr/${TARGET}/bin/SDL2.dll -DSDL2_IMAGE_LIBRARY=/usr/${TARGET}/bin/SDL2_image.dll"
                fi
                ;;

            linux32)
                TARGET=i386-linux-gnu
                if [ "$MSYSTEM" = "" ]; then
                    CMAKE_EXTRA="-DCMAKE_TOOLCHAIN_FILE=../../cmake/Toolchains/linux_i686.toolchain.cmake -DSDL2_INCLUDE_DIR=/usr/include/SDL2 -DSDL2_LIBRARY=/usr/lib/${TARGET}/libSDL2-2.0.so.0 -DSDL2_IMAGE_LIBRARY=/usr/lib/${TARGET}/libSDL2_image-2.0.so.0 -DOPENGL_gl_LIBRARY=/usr/lib/${TARGET}/libGL.so.1 -DOPENGL_glu_LIBRARY=/usr/lib/${TARGET}/libGLU.so.1"
                fi
                ;;

            switch)
                TARGET=aarch64-none-elf
                if [ "$MSYSTEM" = "" ]; then
                    # linux
                    CMAKE_EXTRA="-DCMAKE_TOOLCHAIN_FILE=$DEVKITPRO/cmake/Switch.cmake -DSWITCH=1 -DSDL2_INCLUDE_DIR=$DEVKITPRO/portlibs/switch/include/SDL2 -DSDL2_LIBRARY=$DEVKITPRO/portlibs/switch/lib/libSDL2.a"
                fi
                INCLUDE_DIRECTORIES="$DEVKITPRO/portlibs/switch/include;$DEVKITPRO/libnx/include;$DEVKITPRO/devkitA64/include"
                export INCLUDE_DIRECTORIES
                ;;

            macosx)
                TARGET=x86_64-apple-darwin14
                CMAKE_EXTRA="-DCMAKE_C_FLAGS=-Wno-incompatible-function-pointer-types -DCMAKE_OSX_DEPLOYMENT_TARGET=10.10 -DSDL2_INCLUDE_DIR=${SDKROOT}/../../macports/pkgs/opt/local/include/SDL2 -DSDL2_LIBRARY=${SDKROOT}/../../macports/pkgs/opt/local/lib/libSDL2.dylib -DSDL2_LIBRARIES=${SDKROOT}/../../macports/pkgs/opt/local/lib/libSDL2.dylib -DCMAKE_C_COMPILER=${SDKROOT}/../../bin/o64-clang -DCMAKE_CXX_COMPILER=${SDKROOT}/../../bin/o64-clang++  -DCMAKE_SYSTEM_NAME=Darwin -DCMAKE_OSX_ARCHITECTURES=x86_64 -DCMAKE_OSX_SYSROOT=${SDKROOT}/../../SDK/MacOSX10.10.sdk"
                ;;

            android)
                filtered_args=()
                for arg in "$@"; do
                    [[ "$arg" != "android" ]] && filtered_args+=("$arg")
                done

                build_target armv7a-linux-androideabi "${filtered_args[@]}"
                build_target aarch64-linux-android "${filtered_args[@]}"
                build_target i686-linux-android "${filtered_args[@]}"
                build_target x86_64-linux-android "${filtered_args[@]}"

                exit 0
                ;;

            armv7a-linux-androideabi)
                TARGET=armv7a-linux-androideabi
                ABI=armeabi-v7a
                API=21
                PREFIX_JNI=$PWD/android/$TARGET/$ABI
                CMAKE_EXTRA="-DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake \
                    -DANDROID_ABI=$ABI \
                    -DANDROID_PLATFORM=android-$API \
                    -DPREFIX_JNI=$PREFIX_JNI \
                    -DSDL2_INCLUDE_DIR=$PREFIX_JNI/include/SDL2 \
                    -DSDL2_LIBRARY=$PREFIX_JNI/lib/libSDL2.so"
                ;;

            aarch64-linux-android)
                TARGET=aarch64-linux-android
                ABI=arm64-v8a
                API=21
                PREFIX_JNI=$PWD/android/$TARGET/$ABI
                CMAKE_EXTRA="-DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake \
                    -DANDROID_ABI=$ABI \
                    -DANDROID_PLATFORM=android-$API \
                    -DPREFIX_JNI=$PREFIX_JNI \
                    -DSDL2_INCLUDE_DIR=$PREFIX_JNI/include/SDL2 \
                    -DSDL2_LIBRARY=$PREFIX_JNI/lib/libSDL2.so"
                ;;

            i686-linux-android)
                TARGET=i686-linux-android
                ABI=x86
                API=21
                PREFIX_JNI=$PWD/android/$TARGET/$ABI
                CMAKE_EXTRA="-DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake \
                    -DANDROID_ABI=$ABI \
                    -DANDROID_PLATFORM=android-$API \
                    -DPREFIX_JNI=$PREFIX_JNI \
                    -DSDL2_INCLUDE_DIR=$PREFIX_JNI/include/SDL2 \
                    -DSDL2_LIBRARY=$PREFIX_JNI/lib/libSDL2.so"
                ;;

            x86_64-linux-android)
                TARGET=x86_64-linux-android
                ABI=x86_64
                API=21
                PREFIX_JNI=$PWD/android/$TARGET/$ABI
                CMAKE_EXTRA="-DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake \
                    -DANDROID_ABI=$ABI \
                    -DANDROID_PLATFORM=android-$API \
                    -DPREFIX_JNI=$PREFIX_JNI \
                    -DSDL2_INCLUDE_DIR=$PREFIX_JNI/include/SDL2 \
                    -DSDL2_LIBRARY=$PREFIX_JNI/lib/libSDL2.so"
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
        if [ $ONE_JOB -eq 0 ]; then
            make -j
        else
            make
        fi
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
                ;;

            x86_64-apple-darwin14)
                mkdir -p ../../../../dependencies/$TARGET
                cp -Rf SDL_gpu/lib/* ../../../../dependencies/$TARGET
                ;;

        esac

    fi
    cd -
}

build_target "$@"

echo "### Build done! ###"

exit 0

