#!/bin/sh
cd android/dependencies/jni

#1. Download dependencies.

#SDL2
curl -sS https://libsdl.org/release/SDL2-2.0.5.zip > SDL2.zip
unzip SDL2.zip
rm SDL2.zip
mv SDL2-2.0.5 SDL2

#SDL2_image
curl -sS https://www.libsdl.org/projects/SDL_image/release/SDL2_image-2.0.1.zip > SDL2_image.zip
unzip SDL2_image.zip
rm SDL2_image.zip
mv SDL2_image-2.0.1 SDL2_image

#2. Run ndk-build to generate shared object libraries.
ndk-build

#3. Copy shared object libraries into the app's build chain (these aren't auto-copied by cmake for some reason???).
mkdir ../../app/build/
mkdir ../../app/build/intermediates/
mkdir ../../app/build/intermediates/cmake/
mkdir ../../app/build/intermediates/cmake/debug/
mkdir ../../app/build/intermediates/cmake/debug/obj
cp -r ../libs/* ../../app/build/intermediates/cmake/debug/obj

#4. Run gradlew.sh/gradlew.bat with installDebug to assemble and install the app.
cd ../..
./gradlew installDebug

#5. Auto-launch the new app.
adb shell am start -n org.libsdl.app/org.libsdl.app.SDLActivity

#6. Reset directory.
cd ..
