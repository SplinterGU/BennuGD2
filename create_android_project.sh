#!/bin/bash

if [ $# -lt 2 ]; then
    echo "Uso: $0 GameName GAMEPACKAGEName [icon]"
    exit 1
fi

GAMENAME="$1"
GAMEPACKAGE="$2"
ICON="$3"

GAME=$(echo "$GAMENAME" | sed 's/[^a-zA-Z0-9_]/_/g')
BASE="android_template"
TARGET_DIR="android_projects/$GAME"

echo "Generando proyecto para:"
echo "- Juego: $GAMENAME"
echo "- Paquete: $GAMEPACKAGE"
echo

# Convertir package a ruta (ej: com.mijuego.test -> com/mijuego/test)
PACKAGE_PATH=$(echo "$GAMEPACKAGE" | sed 's/\./\//g')

# Crear estructura de carpetas
GAMEJAVA="$TARGET_DIR/app/src/main/java/$PACKAGE_PATH"

ACTIVITY_CLASSNAME="Activity_$GAME"

mkdir -p "$GAMEJAVA"

# Copiar base del proyecto
mkdir -p "$TARGET_DIR"
cp -r "$BASE/"* "$TARGET_DIR"

cat > "$GAMEJAVA/${ACTIVITY_CLASSNAME}.java" <<EOF
package $GAMEPACKAGE;

import org.libsdl.app.SDLActivity;

public class ${ACTIVITY_CLASSNAME} extends SDLActivity { }
EOF

sed -i "s/org.libsdl.app/${GAMEPACKAGE}/g" "${TARGET_DIR}/app/build.gradle"
sed -i "s/android:name=\"SDLActivity\"/android:name=\"${ACTIVITY_CLASSNAME}\"/g" "${TARGET_DIR}/app/src/main/AndroidManifest.xml"
sed -i "s/Your Game Name/${GAMENAME}/g" "${TARGET_DIR}/app/src/main/res/values/strings.xml"

cp $PWD/build/armv7a-linux-androideabi/bin/*.so "${TARGET_DIR}/app/src/main/jniLibs/armeabi-v7a"
cp $PWD/build/aarch64-linux-android/bin/*.so "${TARGET_DIR}/app/src/main/jniLibs/arm64-v8a"
cp $PWD/build/i686-linux-android/bin/*.so "${TARGET_DIR}/app/src/main/jniLibs/x86"
cp $PWD/build/x86_64-linux-android/bin/*.so "${TARGET_DIR}/app/src/main/jniLibs/x86_64"

cp $PWD/vendor/android/armv7a-linux-androideabi/armeabi-v7a/lib/*.so "${TARGET_DIR}/app/src/main/jniLibs/armeabi-v7a"
cp $PWD/vendor/android/aarch64-linux-android/arm64-v8a/lib/*.so "${TARGET_DIR}/app/src/main/jniLibs/arm64-v8a"
cp $PWD/vendor/android/i686-linux-android/x86/lib/*.so "${TARGET_DIR}/app/src/main/jniLibs/x86"
cp $PWD/vendor/android/x86_64-linux-android/x86_64/lib/*.so "${TARGET_DIR}/app/src/main/jniLibs/x86_64"

cp $PWD/vendor/sdl-gpu/build/armv7a-linux-androideabi/SDL_gpu/lib/libSDL2_gpu.so "${TARGET_DIR}/app/src/main/jniLibs/armeabi-v7a"
cp $PWD/vendor/sdl-gpu/build/aarch64-linux-android/SDL_gpu/lib/libSDL2_gpu.so "${TARGET_DIR}/app/src/main/jniLibs/arm64-v8a"
cp $PWD/vendor/sdl-gpu/build/i686-linux-android/SDL_gpu/lib/libSDL2_gpu.so "${TARGET_DIR}/app/src/main/jniLibs/x86"
cp $PWD/vendor/sdl-gpu/build/x86_64-linux-android/SDL_gpu/lib/libSDL2_gpu.so "${TARGET_DIR}/app/src/main/jniLibs/x86_64"

if [ -n "$ICON" ] && [ -f "$ICON" ]; then
    echo "Redimensionando íconos..."

    BASE_DIR="$TARGET_DIR/app/src/main/res"

    declare -A SIZES=(
        [mipmap-mdpi]=48
        [mipmap-hdpi]=72
        [mipmap-xhdpi]=96
        [mipmap-xxhdpi]=144
        [mipmap-xxxhdpi]=192
    )

    for DIR in "${!SIZES[@]}"; do
        SIZE=${SIZES[$DIR]}
        DEST_DIR="$BASE_DIR/$DIR"
        mkdir -p "$DEST_DIR"
        magick "$ICON" -gravity center -crop 1:1 +repage -resize ${SIZE}x${SIZE} "$DEST_DIR/ic_launcher.png"
        cp "$DEST_DIR/ic_launcher.png" "$DEST_DIR/ic_launcher_round.png"
        cp "$DEST_DIR/ic_launcher.png" "$DEST_DIR/ic_launcher_foreground.png"
        echo "-> $DEST_DIR/ic_launcher*.png ($SIZE x $SIZE)"
    done
else
    echo "No se especificó un ícono válido, se omite la creación de mipmaps."
fi

echo
echo "✔️ Proyecto generado correctamente en: $TARGET_DIR"
echo

echo "Copia tus assets en: $TARGET_DIR/src/main/assets/"
echo
