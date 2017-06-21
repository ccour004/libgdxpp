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
    Matrix4 view,proj,combined;
    ShaderProgram shaderProgram;
    int counter = 0;
    
    Vector3 eye = Vector3(0,0,3),target = Vector3(0,0,0),up = Vector3(0,1,0);
    Vector3 center;
    float near = 0.1f,far = 100.0f,fov = 67.0f,aspect = 0.6182f;
    int width = 640,height = 480;
    
    void buildProj(float near,float far,float fov,float aspect);
    
    void buildView(Vector3& eye,Vector3& target,Vector3& up);
public:
 bool create();

 void resize(int width, int height);

 void render();

 void pause(){}
 void resume(){}
 void dispose(){}
};

