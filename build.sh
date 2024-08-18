#!/bin/bash

show_help() {
    echo "usage:"
    echo "    $0 [windows|windows32|linux|linux32|switch|macosx] [debug] [clean] [packages] [use_sdl2|use_sdl2_gpu] [verbose] [static] [one-job]"
    exit 1
}

fix_install_name() {
    directory=$1
    find "$directory" -type f -print0 | while IFS= read -r -d '' file; do
        if [[ $(file "$file") == *"Mach-O"* ]]; then
            paths=$(x86_64-apple-darwin14-otool -l "$file" | grep -E '(\/opt\/local\/lib|@rpath)' | awk '{print $2}')                        
            while IFS= read -r path; do
                if [[ "$path" == *"@rpath/SDL2_gpu.framework/Versions/A/SDL2_gpu"* ]]; then
                    x86_64-apple-darwin14-install_name_tool -change "$path" "@executable_path/../Frameworks/SDL2_gpu" "$file"
                elif [[ "$path" == *"/opt/local/lib"* ]]; then
                    x86_64-apple-darwin14-install_name_tool -change "$path" "@executable_path/../Libraries$(echo "$path" | sed 's/\/opt\/local\/lib//')" "$file"
                elif [[ "$path" == *@rpath* ]]; then
                    x86_64-apple-darwin14-install_name_tool -change "$path" "@executable_path/$(basename "$path")" "$file"
                fi
            done <<< "$paths"
        fi
    done
}

build_app() {
    app=$1
    mkdir -p packages/$app.app/Contents/Frameworks
    mkdir -p packages/$app.app/Contents/Libraries
    mkdir -p packages/$app.app/Contents/MacOS
    cp -f dependencies/x86_64-apple-darwin14/*.dylib packages/$app.app/Contents/Libraries
    cp -f dependencies/x86_64-apple-darwin14/SDL2_gpu.framework/Versions/Current/SDL2_gpu packages/$app.app/Contents/Frameworks
    cp -f build/x86_64-apple-darwin14/bin/*.dylib build/x86_64-apple-darwin14/bin/$app packages/$app.app/Contents/MacOS

    echo "<?xml version=\"1.0\" encoding=\"UTF-8\"?>
<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">
<plist version=\"1.0\">
<dict>
    <key>CFBundleDevelopmentRegion</key>
    <string>en</string>
    <key>CFBundleExecutable</key>
    <string>$app</string>
    <key>CFBundleIdentifier</key>
    <string>com.bgd2.$app</string>
    <key>CFBundleInfoDictionaryVersion</key>
    <string>6.0</string>
    <key>CFBundleName</key>
    <string>$app</string>
    <key>CFBundlePackageType</key>
    <string>APPL</string>
    <key>CFBundleShortVersionString</key>
    <string>1.0</string>
    <key>CFBundleSignature</key>
    <string>????</string>
    <key>CFBundleVersion</key>
    <string>1</string>
</dict>
</plist>
" > packages/$app.app/Contents/Info.plist

    cd packages

    fix_install_name "$app.app/Contents/Frameworks"
    fix_install_name "$app.app/Contents/Libraries"
    fix_install_name "$app.app/Contents/MacOS"

    tar -zcvf x86_64-apple-darwin14-$app-$(date +"%Y-%m-%d").app.tgz $app.app/*

    rm -Rf $app.app

    cd -
}

#!/bin/bash

# Function to search for the library in the given paths
search_lib() {
    local -n result=$1
    local paths=("${!2}")

    # Initialize the result
    result=""

    # Iterate through the list of paths and search for the library
    for path in "${paths[@]}"; do
        # Search for the corresponding library in the path
        result=$(find "${path}" -name "$4" 2>/dev/null)

        # If the library is found, assign the result and exit the loop
        if [[ -n "${result}" ]]; then
            break
        fi
    done
}

BUILD_TYPE=Release
STATIC_ENABLED=0
LIBRARY_BUILD_TYPE=SHARED
USE_SDL2=0
USE_SDL2_GPU=1
EXTRA_CFLAGS=
MISC_FLAGS=
ONE_JOB=0

args=$@

for i in $args
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

        windows|win)
            TARGET=x86_64-w64-mingw32
            COMPILER="-MINGW"
            SDL2GPUDIR="../../vendor/sdl-gpu/build/$ENV{TARGET}"
            if [ "$MSYSTEM" = "" ]; then
                # linux
                CMAKE_EXTRA="-DCMAKE_TOOLCHAIN_FILE=cmake/Toolchains/Toolchain-cross-mingw32-linux.cmake -DSDL2_INCLUDE_DIR=/usr/${TARGET}/include/SDL2"
            fi
            ;;

        switch)
            TARGET=aarch64-none-elf
            COMPILER=""
            SDL2GPUDIR="../../vendor/sdl-gpu/build/$ENV{TARGET}"
            STATIC_ENABLED=1 # force STATIC
            if [ "$MSYSTEM" = "" ]; then
                # linux
                CMAKE_EXTRA="-DCMAKE_TOOLCHAIN_FILE=$DEVKITPRO/cmake/Switch.cmake -DBUILD_TARGET=interpreter"
            fi
            INCLUDE_DIRECTORIES="$DEVKITPRO/portlibs/switch/include;$DEVKITPRO/libnx/include;$DEVKITPRO/devkitA64/include"
            export INCLUDE_DIRECTORIES
            ;;

        linux)
            TARGET=linux-gnu
            ;;

        ps3)
            TARGET=powerpc64-ps3-elf
            STATIC_ENABLED=1
            USE_SDL2=1
            USE_SDL2_GPU=0
            COMPILER="-mcpu=cell"
            CMAKE_EXTRA="-DPS3_PPU=1 -DSDL2_INCLUDE_DIR=${PS3DEV}/portlibs/ppu/include/SDL2 -DSDL2_LIBRARY=${PS3DEV}/portlibs/ppu/lib/libSDL2.a -DSDL2_IMAGE_INCLUDE_DIR=${PS3DEV}/portlibs/ppu/include/SDL2 -DSDL2_IMAGE_LIBRARY=${PS3DEV}/portlibs/ppu/lib/libSDL2_image.a -DSDL2_MIXER_INCLUDE_DIR=${PS3DEV}/portlibs/ppu/include/SDL2 -DSDLMIXER_LIBRARY=${PS3DEV}/portlibs/ppu/lib/libSDL2_mixer.a -DZLIB_LIBRARY=-lz -DZLIB_INCLUDE_DIR=${PS3DEV}/portlibs/ppu/include -DCMAKE_INCLUDE_PATH=${PS3DEV}/portlibs/ppu/include
                -DBUILD_TARGET=interpreter"
#            -I${PS3DEV}/portlibs/ppu/include 
#            -L${PS3DEV}/portlibs/ppu/lib 
#            -lSDL2_image -lSDL2 -lm -lgcm_sys -lrsx -lsysutil -lrt -llv2 -lio -laudio
            ;;

        windows32|win32)
            TARGET=i686-w64-mingw32
            COMPILER="-MINGW"
            if [ "$MSYSTEM" = "" ]; then
                # linux
                CMAKE_EXTRA="-DBUILD_WIN32=ON -DCMAKE_TOOLCHAIN_FILE=cmake/Toolchains/Toolchain-cross-mingw32-linux.cmake -DSDL2_INCLUDE_DIR=/usr/${TARGET}/include/SDL2 -DSDL2_LIBRARY=/usr/${TARGET}/bin/SDL2.dll -DSDL2_IMAGE_LIBRARY=/usr/${TARGET}/bin/SDL2_image.dll -DSDLMIXER_LIBRARY=/usr/${TARGET}/bin/SDL2_mixer.dll -DSDL2_IMAGE_INCLUDE_DIR=/usr/${TARGET}/include/"
            fi
            ;;

        linux32)
            TARGET=i386-linux-gnu

            if [ "$MSYSTEM" = "" ]; then
                paths=(
                    "/usr/lib/${TARGET}"
                    "/usr/lib32"
                    # Add more paths if necessary
                )

                # Call the function to search for each library
                search_lib SDL2_LIBRARY paths[@] x86_64 "libSDL2.so"
                search_lib SDL2_IMAGE_LIBRARY paths[@] x86_64 "libSDL2_image.so"
                search_lib SDLMIXER_LIBRARY paths[@] x86_64 "libSDL2_mixer.so"

                # linux
                CMAKE_EXTRA="-DCMAKE_TOOLCHAIN_FILE=cmake/Toolchains/linux_i686.toolchain.cmake -DSDL2_INCLUDE_DIR=/usr/include/SDL2 -DSDL2_LIBRARY=${SDL2_LIBRARY} -DSDL2_IMAGE_LIBRARY=${SDL2_IMAGE_LIBRARY} -DSDLMIXER_LIBRARY=${SDLMIXER_LIBRARY}"
            fi
            ;;

        macosx)
            TARGET=x86_64-apple-darwin14
            CMAKE_EXTRA="-DCMAKE_OSX_DEPLOYMENT_TARGET=10.10 -DSDL2_INCLUDE_DIR=${SDKROOT}/../../macports/pkgs/opt/local/include/SDL2 -DSDL2_LIBRARY=${SDKROOT}/../../macports/pkgs/opt/local/lib/libSDL2.dylib -DSDL2_LIBRARIES=${SDKROOT}/../../macports/pkgs/opt/local/lib/libSDL2.dylib -DSDL2_IMAGE_INCLUDE_DIR=${SDKROOT}/../../macports/pkgs/opt/local/include/SDL2 -DSDL2_IMAGE_LIBRARY=${SDKROOT}/../../macports/pkgs/opt/local/lib/libSDL2_image.dylib -DSDL2_Mixer_INCLUDE_DIRS=${SDKROOT}/../../macports/pkgs/opt/local/include/SDL2 -DSDLMIXER_LIBRARY=${SDKROOT}/../../macports/pkgs/opt/local/lib/libSDL2_mixer.dylib -DCMAKE_C_COMPILER=${SDKROOT}/../../bin/o64-clang -DCMAKE_CXX_COMPILER=${SDKROOT}/../../bin/o64-clang++  -DCMAKE_SYSTEM_NAME=Darwin -DCMAKE_OSX_ARCHITECTURES=x86_64 -DCMAKE_OSX_SYSROOT=${SDKROOT}/../../SDK/MacOSX10.10.sdk"
            CMAKE_EXTRA+=" -DCMAKE_C_FLAGS=-Wno-pointer-sign"
            EXTRA_CFLAGS+="-I${SDKROOT}/../../macports/pkgs/opt/local/include"
            STDLIBSFLAGS="-L${SDKROOT}/../../macports/pkgs/opt/local/lib"
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
            if [[ -d build/linux-gnu/bin  ]]; then  tar -zcvf packages/bgd2-linux-gnu-$(date +"%Y-%m-%d").tgz build/linux-gnu/bin/* dependencies/linux-gnu/* WhatsNew.txt --transform='s/[^\/]*\///g'; fi
            if [[ -d build/i386-linux-gnu/bin ]]; then  tar -zcvf packages/bgd2-i386-linux-gnu-$(date +"%Y-%m-%d").tgz build/i386-linux-gnu/bin/* dependencies/i386-linux-gnu/* WhatsNew.txt --transform='s/[^\/]*\///g'; fi
            if [[ -d build/i686-w64-mingw32/bin ]]; then  rar a -ep1 packages/bgd2-i686-w64-mingw32-$(date +"%Y-%m-%d").rar build/i686-w64-mingw32/bin/*.exe build/i686-w64-mingw32/bin/*.dll dependencies/i686-w64-mingw32/* WhatsNew.txt; fi
            if [[ -d build/x86_64-w64-mingw32/bin ]]; then  rar a -ep1 packages/bgd2-x86_64-w64-mingw32-$(date +"%Y-%m-%d").rar build/x86_64-w64-mingw32/bin/*.exe build/x86_64-w64-mingw32/bin/*.dll dependencies/x86_64-w64-mingw32/* WhatsNew.txt; fi
            if [[ -d build/aarch64-none-elf/bin ]]; then  tar -zcvf packages/bgd2-aarch64-none-elf-$(date +"%Y-%m-%d").tgz build/aarch64-none-elf/bin/bgdi.elf WhatsNew.txt  --transform='s/[^\/]*\///g'; fi
            if [[ -d build/x86_64-apple-darwin14/bin ]]; then
                build_app bgdc
                build_app bgdi
                build_app moddesc
            fi        
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
    case $(uname -s|tr '[:upper:]' '[:lower:]') in
        mingw64*)
            TARGET=x86_64-w64-mingw32
            COMPILER="-MINGW"
            SDL2GPUDIR="../../vendor/sdl-gpu/build/$ENV{TARGET}"
            if [ "$MSYSTEM" = "" ]; then
                # linux
                CMAKE_EXTRA="-DCMAKE_TOOLCHAIN_FILE=cmake/Toolchains/Toolchain-cross-mingw32-linux.cmake -DSDL2_INCLUDE_DIR=/usr/x86_64-w64-mingw32/include/SDL2"
            fi
            ;;

        linux)
            TARGET=linux-gnu
            ;;
    esac

    if [ "$TARGET" == "" ]
    then
        show_help
    fi
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
    cd core
    ./make-fakedl.sh
    cd -
fi

export PKG_CONFIG_PATH
export TARGET
export SDL2GPUDIR
export SDL2GPULIBDIR
export COMPILER
export LIBRARY_BUILD_TYPE

if [ "$CLEAN" == "1" ]
then
    echo "### Cleaning previous build ($TARGET) ###"
    rm -rf build/$TARGET
fi

echo "### Building BennuGD ($TARGET) ###"
mkdir -p build/$TARGET 2>/dev/null
cd build/$TARGET
if [ "$TARGET" == "powerpc64-ps3-elf" ]
then
    AS=ppu-as \
    CC=ppu-gcc \
    CXX=ppu-g++ \
    AR=ppu-ar \
    LD=ppu-gcc \
    STRIP=ppu-strip \
    OBJCOPY=ppu-objcopy \
    PATH=$PS3DEV/bin:$PS3DEV/ppu/bin:$PATH \
    PORTLIBS=$PS3DEV/portlibs/ppu \
    CFLAGS_INIT="" \
    CXXFLAGS_INIT="-D_GLIBCX11_USE_C99_STDIO" \
    CMAKE_INCLUDE_DIRECTORIES=$PS3DEV/ppu/include:$PS3DEV/ppu/include/simdmath \
    CMAKE_LIBRARY_DIRECTORIES=$PS3DEV/ppu/lib \
    CMAKE_PREFIX_PATH=$PS3DEV/ppu \
    cmake ../.. $DEBUG ${CMAKE_CXX_COMPILER} -DINCLUDE_DIRECTORIES="${INCLUDE_DIRECTORIES}" -DCMAKE_BUILD_TYPE=$BUILD_TYPE $CMAKE_EXTRA $VERBOSE -DEXTRA_CFLAGS="$EXTRA_CFLAGS" $MISC_FLAGS -DLIBRARY_BUILD_TYPE=$LIBRARY_BUILD_TYPE -DSTDLIBSFLAGS="${STDLIBSFLAGS}"
else
    cmake ../.. $DEBUG ${CMAKE_CXX_COMPILER} -DINCLUDE_DIRECTORIES="${INCLUDE_DIRECTORIES}" -DCMAKE_BUILD_TYPE=$BUILD_TYPE $CMAKE_EXTRA $VERBOSE -DEXTRA_CFLAGS="$EXTRA_CFLAGS" $MISC_FLAGS -DLIBRARY_BUILD_TYPE=$LIBRARY_BUILD_TYPE -DSTDLIBSFLAGS="${STDLIBSFLAGS}"
fi
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
