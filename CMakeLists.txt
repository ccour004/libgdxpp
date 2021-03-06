cmake_minimum_required (VERSION 3.0.2)

if (ANDROID)
    message(STATUS "Hello from Android build!")
endif()

project (gdxpp)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

#SDL2
find_package(SDL2 REQUIRED)
if(SDL2_FOUND)
    include_directories(${SDL2_INCLUDE_DIR})
endif()

#SDL_image
find_package(SDL2_image REQUIRED)
if(SDL2IMAGE_FOUND)
    include_directories(${SDL2IMAGE_INCLUDE_DIR})
endif()

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/ SOURCE)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/math/ MATH_SOURCE)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/math/collision MATH_COLLISION_SOURCE)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/ GRAPHICS_SOURCE)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/glutils GLUTILS_SOURCE)

add_library(gdxpp SHARED ${SOURCE} ${MATH_SOURCE} ${GRAPHICS_SOURCE} ${MATH_COLLISION_SOURCE} ${GLUTILS_SOURCE})
target_compile_definitions(gdxpp PRIVATE DESKTOP=1)
