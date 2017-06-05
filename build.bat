mkdir dependencies
cd dependencies

::SDL2
curl -sS https://libsdl.org/release/SDL2-2.0.5.zip > SDL2-2.0.5.zip
7z x SDL2-2.0.5.zip
del SDL2-2.0.5.zip
mkdir SDL2-2.0.5-build
cd SDL2-2.0.5-build
cmake ../SDL2-2.0.5
make

::bullet3
cd ..
git clone https://github.com/bulletphysics/bullet3.git
mkdir bullet3-build
cd bullet3-build
cmake ../bullet3
make

::desktop
cd ../..
cmake .
cmake --build . --target Core --config Debug
cmake --build . --target Core --config Release

::android
cd android
gradlew :SDL2:distributeLib
gradlew :bullet3:distributeLib
gradlew :main:distributeLib
gradlew assembleDebug

::launch android app and desktop app
adb uninstall org.libsdl.app
adb install app/build/outputs/apk/app-all-debug.apk
adb shell am start -n org.libsdl.app/org.libsdl.app.SDLActivity
cd ..
Release\Core

