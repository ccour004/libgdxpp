cmake_minimum_required (VERSION 3.0.2)

if (ANDROID)
    message(STATUS "Hello from Android build!")
endif()

project (Core)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
add_executable(Core "${CMAKE_CURRENT_SOURCE_DIR}/core/src/main.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/core/src/LibGDXApplication.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/core/src/GL.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/core/src/MyAppListener.cpp"
"${CMAKE_SOURCE_DIR}/core/src/graphics/glutils/ShaderProgram.cpp"
"${CMAKE_SOURCE_DIR}/core/src/graphics/glutils/IndexData.cpp"
"${CMAKE_SOURCE_DIR}/core/src/graphics/glutils/VertexData.cpp"
"${CMAKE_SOURCE_DIR}/core/src/graphics/VertexAttributes.cpp"
"${CMAKE_SOURCE_DIR}/core/src/graphics/Mesh.cpp"
"${CMAKE_SOURCE_DIR}/core/src/math/Matrix4.cpp"
"${CMAKE_SOURCE_DIR}/core/src/math/Affine2.cpp"
"${CMAKE_SOURCE_DIR}/core/src/math/Vector2.cpp"
"${CMAKE_SOURCE_DIR}/core/src/math/Vector3.cpp"
"${CMAKE_SOURCE_DIR}/core/src/math/Frustum.cpp"
"${CMAKE_SOURCE_DIR}/core/src/math/MathUtils.cpp"
"${CMAKE_SOURCE_DIR}/core/src/math/RandomXS128.cpp"
"${CMAKE_SOURCE_DIR}/core/src/math/collision/BoundingBox.cpp"
"${CMAKE_SOURCE_DIR}/core/src/math/collision/Ray.cpp"
"${CMAKE_SOURCE_DIR}/core/src/math/Quaternion.cpp")

#Boost
#find_package(Boost 1.62.0 REQUIRED)
#if(Boost_FOUND)
#    include_directories(${Boost_INCLUDE_DIR})
#    target_link_libraries(Core ${Boost_LIBRARIES})
#endif()

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

#SDL_image
find_package(SDL2_image REQUIRED)
if(SDL2IMAGE_FOUND)
    include_directories(${SDL2IMAGE_INCLUDE_DIR})
    target_link_libraries(Core ${SDL2IMAGE_LIBRARY})
endif()

#Bullet Physics (v.3., but only using v.2 CPU-based code)
#include_directories(${CMAKE_SOURCE_DIR}/dependencies/bullet3/src)
#target_link_libraries(Core 
#        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/bullet3-build/src/BulletDynamics/libBulletDynamics.a
#        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/bullet3-build/src/BulletCollision/libBulletCollision.a
#        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/bullet3-build/src/LinearMath/libLinearMath.a)

target_compile_definitions(Core PRIVATE DESKTOP=1)
