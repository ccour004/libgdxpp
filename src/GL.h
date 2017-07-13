#pragma once

#ifdef DESKTOP
	#include <GL/glew.h> 
    #include <SDL_opengl.h> 
	#include <GL/glu.h>
#else
    //#include <SDL_opengl.h>
    //#include <video/android/SDL_androidvideo.h>
    #include <GLES3/gl3.h>
#endif

#include <SDL.h>
#include <SDL_image.h>
#include <sstream>

char* file_read(const char* filename);
std::string fileToString(const std::string& filename);

class Color{
public:
    Color(float r,float g,float b,float a){this->r = r;this->g = g;this->b = b;this->a = a;}
    float r,g,b,a;
};

