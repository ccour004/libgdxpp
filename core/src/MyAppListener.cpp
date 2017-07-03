#include "MyAppListener.h"
#include <iostream>
#include <chrono>
#include "OrthographicCamera.h"

bool MyAppListener::create(){
     if(isCreated) return true;
        //Setup shader program.
        ShaderProgram::prependVertexCode = "#version 300 es\n";
        ShaderProgram::prependFragmentCode = "#version 300 es\n";
        shaderProgram.push_back(ShaderProgram(
            fileToString("assets/default.vert"),fileToString("assets/default.frag"),"default"));
        shaderProgram.push_back(ShaderProgram(
            fileToString("assets/alt.vert"),fileToString("assets/alt.frag"),"default"));
            
        for(ShaderProgram program:shaderProgram)
            if(!program.isCompiled()) return false;
            
        shaderProgram[1].setAttributef("a_color",0,1,0,1);
        
        center = Vector3(0,0,0);
        camera = PerspectiveCamera(67,640,480);
        camera.near = 0.01f;
        camera.far = 200;
        camera.position.set(0,0,3);
        camera.lookAt(center);
        camera.update();
        
        std::vector<VertexAttribute> attr{VertexAttribute::position()};
        std::vector<GLfloat> vertices = std::vector<GLfloat>{
    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};

        std::vector<GLuint> indices(vertices.size());
        for(int i = 0;i < vertices.size();i++){
            vertices[i] -= 0.5f;
            indices[i] = i;
        }
        tempMeshes.push_back(Mesh(VertexData(VERTEX_BUFFER_OBJECT,true,vertices.size(),attr),
                        IndexData(INDEX_BUFFER_OBJECT,true,indices.size()),vertices,indices,true));
        
        //Setup instance.
        modelBuilder = MeshBuilder();
        modelBuilder.build(2,2,2,20,20);
        tempMeshes.push_back(modelBuilder.meshes[0]);
        isCreated = true;
        //SDL_Log("MESH: %s",modelBuilder.meshes[0].toString().c_str());
        return true;
 }
 
  void MyAppListener::render(){
    glViewport(0,0,width,height);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    /*camera.translate(0.0f,0.0f,0.1f);
      camera.update();*/
      std::chrono::time_point<std::chrono::system_clock> now =
        std::chrono::system_clock::now();
    std::chrono::duration<double> delta = now - lastTime;lastTime = now;
      camera.rotateAround(Vector3(0,0,0),Vector3(0,1,0),
        std::chrono::duration_cast<std::chrono::milliseconds>(delta).count() * 0.1f);
    camera.update();

    //Bind program
    shaderProgram[0].begin();

    //Set transform
    shaderProgram[0].setUniformMatrix("u_projView",camera.combined);
        
    //Render mesh w/ auto-bind.
    modelBuilder.meshes[0].render(shaderProgram[0],GL_TRIANGLES);

    //Unbind program
    shaderProgram[0].end();
    
        //Bind program
    shaderProgram[1].begin();

    //Set transform
    shaderProgram[1].setUniformMatrix("u_projView",camera.combined);
        
    //Render mesh w/ auto-bind.
    tempMeshes[0].render(shaderProgram[1],GL_TRIANGLES);

    //Unbind program
    shaderProgram[1].end();
 }
 
 void MyAppListener::resize(int width, int height){
    this->width = width;this->height = height;
    camera.viewportWidth = width;
    camera.viewportHeight = height;
    camera.lookAt(center);
    camera.update();
 }
 
 

