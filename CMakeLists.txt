cmake_minimum_required (VERSION 2.4.6)
    if(COMMAND cmake_policy)
      cmake_policy(SET CMP0003 NEW)
    endif(COMMAND cmake_policy)
project (Core)
include_directories(${CMAKE_SOURCE_DIR}/dependencies/SDL2-2.0.5/include)
include_directories(${CMAKE_SOURCE_DIR}/dependencies/bullet3/src)
add_executable(Core ${CMAKE_SOURCE_DIR}/core/src/main.cpp)
link_directories(${CMAKE_SOURCE_DIR}/dependencies/bullet3-build/src/LinearMath/)
target_link_libraries(Core 
	${CMAKE_SOURCE_DIR}/dependencies/SDL2-2.0.5-build/libSDL2-2.0.so
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/bullet3-build/src/BulletDynamics/libBulletDynamics.a
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/bullet3-build/src/BulletCollision/libBulletCollision.a
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/bullet3-build/src/LinearMath/libLinearMath.a
-lGL -lGLU -lGLEW)
target_compile_definitions(Core PRIVATE DESKTOP=1)
