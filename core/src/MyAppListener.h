#pragma once

#include "GL.h"
#include "math/Vector3.h"
#include "PerspectiveCamera.h"
#include "ShapeCreator.hpp"
#include "LibGDXApplication.h"
#include "graphics/glutils/ShaderProgram.h"
#include "graphics/Mesh.h"
#include <chrono>

class MyAppListener: public ApplicationListener{
    MeshBuilder modelBuilder;
    bool isCreated = false;
    PerspectiveCamera camera;
    std::vector<ShaderProgram> shaderProgram;
    int counter = 0;
    
    Vector3 center = Vector3();
    float near = 0.1f,far = 100.0f,fov = 67.0f;
    int width = 640,height = 480;
    
    std::vector<Mesh> tempMeshes;
    std::chrono::time_point<std::chrono::system_clock> lastTime;
public:
 bool create();
 void resize(int width, int height);
 void render();
 void pause(){}
 void resume(){}
 void dispose(){}
};

