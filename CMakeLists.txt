cmake_minimum_required (VERSION 3.0.2)

project (Core)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(MSYS_DEP ${MSYS_DEP} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/win_x86_64")
add_executable(Core "${CMAKE_CURRENT_SOURCE_DIR}/core/src/main.cpp")

#OPENGL (GL and GLU)
find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
	include_directories(${OPENGL_INCLUDE_DIR})
	target_link_libraries(Core ${OPENGL_LIBRARIES})
endif()

if(WIN32)
	#set(CMAKE_EXE_LINKER_FLAGS
    #  "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:msvcrtd.lib /NODEFAULTLIB:LIBCMT")
    include_directories("${MSYS_DEP}/SDL2-2.0.5/include/SDL2" "${MSYS_DEP}/glew/include")
	include_directories("${CMAKE_CURRENT_SOURCE_DIR}/dependencies/bullet3/src")
	target_link_libraries(Core "${MSYS_DEP}/glew/lib/glew32d.lib"
	    "${MSYS_DEP}/SDL2-2.0.5/lib/Debug/SDL2main.lib" "${MSYS_DEP}/SDL2-2.0.5/lib/Debug/SDL2.lib"
	    "${MSYS_DEP}/bullet3/lib/Release/BulletDynamics.lib" "${MSYS_DEP}/bullet3/lib/Release/BulletCollision.lib" 
		"${MSYS_DEP}/bullet3/lib/Release/LinearMath.lib")
else()
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
endif()

target_compile_definitions(Core PRIVATE DESKTOP=1)
