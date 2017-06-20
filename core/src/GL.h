#pragma once

#ifdef DESKTOP
	#include <GL/glew.h> 
    #include <SDL_opengl.h> 
	#include <GL/glu.h>
#else
	#include <GLES3/gl3.h>
#endif

#include <SDL.h>
#include <sstream>

char* file_read(const char* filename) {
        SDL_RWops *rw = SDL_RWFromFile(filename, "rb");
        if (rw == NULL) return NULL;

        Sint64 res_size = SDL_RWsize(rw);
        char* res = (char*)malloc(res_size + 1);

        Sint64 nb_read_total = 0, nb_read = 1;
        char* buf = res;
        while (nb_read_total < res_size && nb_read != 0) {
                nb_read = SDL_RWread(rw, buf, 1, (res_size - nb_read_total));
                nb_read_total += nb_read;
                buf += nb_read;
        }
        SDL_RWclose(rw);
        if (nb_read_total != res_size) {
                free(res);
                return NULL;
        }

        res[nb_read_total] = '\0';
        return res;
}

std::string fileToString(const std::string& filename){
    return std::string(file_read(filename.c_str()));
}

class Color{
public:
    Color(float r,float g,float b,float a){this->r = r;this->g = g;this->b = b;this->a = a;}
    float r,g,b,a;
};

