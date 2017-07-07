#include "MyAppListener.h"
#include <iostream>
#include <memory>
#include <chrono>

bool MyAppListener::create(){
     if(isCreated) return true;
     
        //Texture creation test
        image = IMG_Load("assets/test.png");
        if(!image)
            SDL_Log("IMG_Load: %s\n",IMG_GetError());
            
        glEnable (GL_BLEND); glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        int format = GL_RGB;
        switch(image->format->BytesPerPixel){
            case 3: format = GL_RGB; break;
            case 4: format = GL_RGBA; break;
        }
        
        SDL_Log("FORMAT: %i (%i,%i,%i,%i)",image->format->BytesPerPixel,image->format->Rmask,image->format->Gmask,image->format->Bmask,image->format->Amask);
            
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        
        glTexImage2D(GL_TEXTURE_2D, 0, format, image->w, image->h, 0, format, GL_UNSIGNED_BYTE, image->pixels);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
     
        //Setup shader program.
        ShaderProgram::prependVertexCode = "#version 300 es\n";
        ShaderProgram::prependFragmentCode = "#version 300 es\n";
        shaderProgram.push_back(ShaderProgram(
            fileToString("assets/default.vert"),fileToString("assets/default.frag"),"default"));
        shaderProgram.push_back(ShaderProgram(
            fileToString("assets/alt.vert"),fileToString("assets/alt.frag"),"default"));
        shaderProgram.push_back(ShaderProgram(
            fileToString("assets/texTest.vert"),fileToString("assets/texTest.frag"),"default"));
            
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
        
        std::vector<VertexAttribute> attr{VertexAttribute::position()},
            attr2{VertexAttribute::position(),VertexAttribute::texCoords(0)};
        std::vector<GLfloat> vertices{
                        -1,1,-2,   0,0,
                        1,1,-2,    1,0,
                        1,1,0,     1,0,
                        -1,1,0,    0,0,
                        -1,-1,-2,  0,1,
                        1,-1,-2,   1,1,
                        1,-1,0,    1,1,
                        -1,-1,0    ,0,1
        };

        std::vector<GLuint> indices{
            3,1,0,2,1,3, 
            0,5,4,1,5,0,
            3,4,7,0,4,3,
            1,6,5,2,6,1,
            2,7,6,3,7,2,
            6,4,5,7,4,6
        };
        
        meshes.push_back(std::make_shared<Mesh>(VERTEX_BUFFER_OBJECT,vertices,attr2,
                    INDEX_BUFFER_OBJECT,indices,true,true));
        
        //Setup instance.
        vertices = std::vector<GLfloat>();
        indices = std::vector<GLuint>();
        MeshBuilder::build(vertices,indices,2,2,2,20,20);
        meshes.push_back(std::make_shared<Mesh>(VERTEX_BUFFER_OBJECT,vertices,attr,
                    INDEX_BUFFER_OBJECT,indices,true,true));
        
        isCreated = true;
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

    //
    shaderProgram[1].begin();
    shaderProgram[1].setUniformMatrix("u_projView",camera.combined);
    meshes[1]->render(shaderProgram[1],GL_TRIANGLES);
    shaderProgram[1].end();
    
    //
    shaderProgram[2].begin();
    shaderProgram[2].setUniformMatrix("u_projView",camera.combined);
    meshes[0]->render(shaderProgram[2],GL_TRIANGLES);
    shaderProgram[2].end();
 }
 
 void MyAppListener::resize(int width, int height){
    this->width = width;this->height = height;
    camera.viewportWidth = width;
    camera.viewportHeight = height;
    camera.lookAt(center);
    camera.update();
 }
 
 void MyAppListener::dispose(){
     SDL_Log("DISPOSE CALLED!");
     delete image;
 }