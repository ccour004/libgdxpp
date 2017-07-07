
#Specify the toolchain as '4.9' to use gcc instead of clang (clang fails currently).
NDK_TOOLCHAIN_VERSION := 4.9
APP_PLATFORM := android-25
APP_CPPFLAGS += -std=c++14
APP_STL := c++_static
APP_ABI := arm64-v8a,armeabi-v7a
