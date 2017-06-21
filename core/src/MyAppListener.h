#pragma once

#include "GL.h"
#include "math/Vector3.h"
#include "PerspectiveCamera.h"
#include "ShapeCreator.hpp"
#include "LibGDXApplication.h"
#include "ShaderProgram.h"

class MyAppListener: public ApplicationListener{
    MeshBuilder modelBuilder = MeshBuilder();
    bool isCreated = false;
    Mesh mesh;
    PerspectiveCamera camera;
    ShaderProgram shaderProgram;
    int counter = 0;
    
    Vector3 center = Vector3();
    float near = 0.1f,far = 100.0f,fov = 67.0f;
    int width = 640,height = 480;
public:
 bool create();
 void resize(int width, int height);
 void render();
 void pause(){}
 void resume(){}
 void dispose(){}
};

