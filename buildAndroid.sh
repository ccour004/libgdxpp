#!/bin/sh
#android
cd android
./gradlew :SDL2:distributeLib
./gradlew :bullet3:distributeLib
./gradlew :main:distributeLib
./gradlew assembleDebug

#launch android app and desktop app
adb uninstall org.libsdl.app
adb install app/build/outputs/apk/app-all-debug.apk
adb shell am start -n org.libsdl.app/org.libsdl.app.SDLActivity

