

#include "ShapeCreator.hpp"
#include "LibGDXApplication.h"

MeshBuilder modelBuilder;
Shader shader;
Mesh mesh;
MeshInstance* instance;

class MyAppListener: public ApplicationListener{
int width = 640,height = 480;
public:
 void create(){
        //Init meshes.
        shader.init();
        mesh = modelBuilder.build(2,2,2,20,20);
        mesh.init();

        //Setup instance.
        shader.setModelMatrix(0,0,0);
        shader.setProjectionMatrix(0.1f,100.0f,0.6182f,67.0f);
        shader.setViewMatrix(btVector3(0,0,3.0f),
                      btVector3(0,0,0),
                      btVector3(0,1.0f,0));
        instance = new MeshInstance(&mesh,&shader,btVector3(0,0,0));
 }

 void resize(int width, int height){
	instance->resize(width,height);
	this->width = width;this->height = height;
 }

 void render(){
    glViewport(0,0,width,height);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    instance->render();
 }

 void pause(){}
 void resume(){}
 void dispose(){
    delete instance;
 }
};

int main(int argc, char* argv[])
{
    LibGDX_Application* app = new LibGDX_Application(new MyAppListener());
    return 0;
}
