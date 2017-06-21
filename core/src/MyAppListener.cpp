#include "MyAppListener.h"

bool MyAppListener::create(){
     if(isCreated) return true;
        //Setup shader program.
        ShaderProgram::prependVertexCode = "#version 300 es\n";
        ShaderProgram::prependFragmentCode = "#version 300 es\n";
        shaderProgram = ShaderProgram(
            fileToString("assets/default.vert"),fileToString("assets/default.frag"),"default");
            
        if(!shaderProgram.isCompiled()) return false;
        
        center = Vector3(0,0,0);
        camera = PerspectiveCamera(67,640,480,640,480);
        camera.position.set(0,0,3);
        camera.lookAt(center);
        camera.far = 200;
        camera.update(640,480);
        
        //Setup instance.
        mesh = modelBuilder.build(2,2,2,20,20);
        mesh.init();
        isCreated = true;
        return true;
 }
 
  void MyAppListener::render(){
    glViewport(0,0,width,height);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //camera.transform(0.001f,0,0);

    //Bind program
    shaderProgram.begin();

    //Set transform
    shaderProgram.setUniformMatrix("u_projView",camera.combined);
            
    //Enable vertex position
    shaderProgram.enableVertexAttribute(ShaderProgram::POSITION_ATTRIBUTE);

    //Set vertex data
    glBindBuffer( GL_ARRAY_BUFFER, mesh.gVBO );
    shaderProgram.setVertexAttribute(ShaderProgram::POSITION_ATTRIBUTE,3, GL_FLOAT, GL_FALSE,3 * sizeof(GLfloat));

    //Render mesh one time.
    mesh.render();

    //Disable vertex position
    shaderProgram.disableVertexAttribute(ShaderProgram::POSITION_ATTRIBUTE);

    //Unbind program
    shaderProgram.end();
 }
 
 void MyAppListener::resize(int width, int height){
    this->width = width;this->height = height;
    camera.viewportWidth = width;
    camera.viewportHeight = height;
    camera.lookAt(center);
    camera.update(width,height);
 }
 
 

