# libgdxpp
Porting portions of LibGDX to C/C++.

* Run build.sh to retrieve and build dependencies, build android/desktop apps, and launch everything.
  * Dependencies required for desktop: GLEW (GL extension wrangler), OpenGL, GLU.
  * Dependencies required for android: none (should be self-contained).
* augmented from from android-sdl2-gradle-template:
  * Requires a folder 'dependencies'
    * put SDL2-2.0.5 in it
    * put bullet3 in it
  * Requires a local.properties file
    * ndk.dir= /path/to/sdk/ndk-bundle
    * sdk.dir= /path/to/sdk
  * Build SDL2: ./gradlew :SDL2:distributeLib
  * Build Bullet Physics (only CPU-based (v.2) for now pulled out of v.3 repo): ./gradlew :bullet3:distributeLib
  * Build main/core: ./gradlew :main:distributeLib (re-run this after editing any files in core)
  * Assemble APK: ./gradlew assembleDebug (re-run this after editing any files in core)
  * Install the APK: adb install app/build/outputs/apk/app-all-debug.apk (smaller platform-specific APKS will be in the same directory)
