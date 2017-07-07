# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

cmake_minimum_required(VERSION 3.4.1)
set( CMAKE_CXX_STANDARD 14 )

find_library(GLES2_LIBRARY libGLESv2.so HINTS ${GLES2_INCLUDE_DIR}/../lib)
find_library(GLES3_LIBRARY libGLESv3.so HINTS ${GLES3_INCLUDE_DIR}/../lib)

set(MY_DIR /home/colin/git/libgdxpp/android/dependencies/libs)

#SDL2
add_library(SDL2 SHARED IMPORTED )
set_target_properties(SDL2 PROPERTIES IMPORTED_LOCATION ${MY_DIR}/${ANDROID_ABI}/libSDL2.so)
include_directories(SDL2/include)

#SDL2_image
add_library(SDL2_image SHARED IMPORTED )
set_target_properties(SDL2_image PROPERTIES IMPORTED_LOCATION ${MY_DIR}/${ANDROID_ABI}/libSDL2_image.so)
include_directories(SDL2_image)

#gdxpp
add_library(gdxpp SHARED IMPORTED )
set_target_properties(gdxpp PROPERTIES IMPORTED_LOCATION ${MY_DIR}/${ANDROID_ABI}/libgdxpp.so)
include_directories(gdxpp)

#main
add_library(main SHARED SDL2/src/main/android/SDL_android_main.c)

#Link everything together.
target_link_libraries(main gdxpp SDL2 SDL2_image ${GLES2_LIBRARY} ${GLES3_LIBRARY})
