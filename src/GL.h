#pragma once

#ifdef DESKTOP
	#include <GL/glew.h> 
    #include <SDL_opengl.h> 
	#include <GL/glu.h>
#else
    #include <GLES3/gl3.h>
#endif

#include <SDL.h>
#include <SDL_image.h>
#include <sstream>

char* file_read(const char* filename);
std::string fileToString(const std::string& filename);