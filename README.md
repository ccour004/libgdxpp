# libgdxpp
Porting portions of LibGDX to C++, using standard libraries and C++11/14. Use SDL2 and its dependent projects for graphics, input, etc. for maximum portability across desktop and mobile (Android toolchain provided, still need iOS toolchain).

Usage/building:
* Run build.sh to retrieve and build dependencies, build android/desktop apps, and launch everything.
* Desktop toolchain uses cmake, and searches for dependencies on the build machine.
  * Required tools: cmake, dependencies: GLEW (GL extension wrangler), OpenGL, GLU, SDL2, SDL2_image.
  * Build, install, and auto-launch the Desktop app: buildDesktop.sh.
* Android toolchain uses ndk-build to generate shared libraries, and gradle w/ cmake to package everything together.
  * Required tools: Android SDK (install cmake, ndk-bundle including ndk-build), dependencies (SDL2, SDL2_image) will be downloaded automatically.
  * Requires a local.properties file in the 'android' folder with two lines specifying the NDK and SDK paths:
    * ndk.dir= /path/to/sdk/ndk-bundle
    * sdk.dir= /path/to/sdk
  * Build, install, and auto-launch the Android app: buildAndroid.sh.
  * Add new ABIs (besides armv7 (32 bit) and armv8 (64 bit)) by adding ABIs to android/dependencies/jni/Android.mk and adding abiFilter entries to android/app/build.gradle. Also, change the min and target SDK, and other typical Android properties in this gradle file.
* Note that the shell scripts assume a Unix/Linux environment, but could fairly easily be altered for a Windows environment.

Developing:
* Put all game assets in the top-level 'assets' folder, including shaders, texture files, etc.
* For now, edit core/src/MyAppListener.h and core/src/MyAppListener.cpp to add game logic in a similar fashion to LibGDX. In the future, the libgdxpp code and the toolchain should be separated. 
