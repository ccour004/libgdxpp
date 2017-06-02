cmake_minimum_required (VERSION 3.0.2)

project (Core)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
add_executable(Core "${CMAKE_CURRENT_SOURCE_DIR}/core/src/main.cpp")

#OPENGL (GL and GLU)
find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
    include_directories(${OPENGL_INCLUDE_DIR})
    target_link_libraries(Core ${OPENGL_LIBRARIES})
endif()

#GLEW
find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    target_link_libraries(Core ${GLEW_LIBRARIES})
endif()

#SDL2
find_package(SDL2 REQUIRED)
if(SDL2_FOUND)
    include_directories(${SDL2_INCLUDE_DIR})
    target_link_libraries(Core ${SDL2_LIBRARY})
endif()

#Bullet Physics (v.3., but only using v.2 CPU-based code)
include_directories(${CMAKE_SOURCE_DIR}/dependencies/bullet3/src)
target_link_libraries(Core 
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/bullet3-build/src/BulletDynamics/libBulletDynamics.a
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/bullet3-build/src/BulletCollision/libBulletCollision.a
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/bullet3-build/src/LinearMath/libLinearMath.a)

target_compile_definitions(Core PRIVATE DESKTOP=1)
