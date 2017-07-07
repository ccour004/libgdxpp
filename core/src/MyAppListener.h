#pragma once

#include "GL.h"
#include "math/Vector3.h"
#include "PerspectiveCamera.h"
#include "ShapeCreator.hpp"
#include "LibGDXApplication.h"
#include "graphics/glutils/ShaderProgram.h"
#include "graphics/Mesh.h"
#include <chrono>
#include <memory>

class MyAppListener: public ApplicationListener{
    bool isCreated = false;
    PerspectiveCamera camera;
    std::vector<ShaderProgram> shaderProgram;
    int counter = 0;
    
    SDL_Surface* image;
    GLuint tex;
    
    Vector3 center;
    float near = 0.1f,far = 100.0f,fov = 67.0f;
    int width = 640,height = 480;
    
    std::vector<std::shared_ptr<Mesh>> meshes;
    std::chrono::time_point<std::chrono::system_clock> lastTime;
public:
 bool create();
 void resize(int width, int height);
 void render();
 void pause(){}
 void resume(){}
 void dispose();
};

