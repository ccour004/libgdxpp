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
    Matrix4 view,proj;
    ShaderProgram shaderProgram;
    int counter = 0;
    
    Vector3 eye = Vector3(0,0,3),target = Vector3(0,0,0),up = Vector3(0,1,0);
    float near = 0.1f,far = 100.0f,fov = 67.0f,aspect = 0.6182f;
    int width = 640,height = 480;
    
    void buildProj(float near,float far,float fov,float aspect){
        float top = near * tan(M_PI / 180.0f * fov / 2.0f),bottom = -top,right = top * aspect, left = -right;
        proj = Matrix4(std::vector<float>{
        (2.0f * near) / (right-left),0,(right+left)/(right-left),0,
        0,(2.0f * near)/(top-bottom),(top+bottom)/(top-bottom),0,
        0,0,-1.0f * ((far+near)/(far-near)),-(2.0f * far*near) / (far-near),
        0,0,-1,1
        });        
    }
    
    void buildView(Vector3& eye,Vector3& target,Vector3& up){
        Vector3 zaxis = (eye-target).nor();
        Vector3 xaxis = up.crs(zaxis).nor();
        Vector3 yaxis = zaxis.crs(xaxis); 
        SDL_Log("XAXIS: %s,YAXIS: %s,ZAXIS: %s",xaxis.toString().c_str(),yaxis.toString().c_str(),
            zaxis.toString().c_str());
        SDL_Log("XAXIS DOT EYE: %f,YAXIS DOT EYE: %f,ZAXIS DOT EYE: %f",-xaxis.dot(eye),-yaxis.dot(eye),-zaxis.dot(eye));
        view = Matrix4(std::vector<float>{
            xaxis.x,yaxis.x,zaxis.x,0,
            xaxis.y,yaxis.y,zaxis.y,0,
            xaxis.z,yaxis.z,zaxis.z,0,
            -xaxis.dot(eye),-yaxis.dot(eye),-zaxis.dot(eye),1.0f
        });
    }
public:
 bool create(){
     if(isCreated) return true;
        //Setup shader program.
        ShaderProgram::prependVertexCode = "#version 300 es\n";
        ShaderProgram::prependFragmentCode = "#version 300 es\n";
        shaderProgram = ShaderProgram(
            fileToString("assets/default.vert"),fileToString("assets/default.frag"),"default");
            
        if(!shaderProgram.isCompiled()) return false;
        
        //Build proj,view matrices;
        buildProj(near,far,fov,aspect);
        buildView(eye,target,up);
        
        SDL_Log("EYE: %s,VIEW: %s",eye.toString().c_str(),view.toString().c_str());
        //Setup instance.
        mesh = modelBuilder.build(2,2,2,20,20);
        mesh.init();
        isCreated = true;
        return true;
 }

 void resize(int width, int height){
    this->width = width;this->height = height;
    aspect = ((float)width)/((float)height);
    buildProj(near,far,fov,aspect);
 }

 void render(){
    glViewport(0,0,width,height);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    eye.add(0.001f,0,0);
    buildView(eye,target,up);

    //Bind program
    shaderProgram.begin();

    //Set transform
    SDL_Log("PROJ: %s,VIEW: %s",proj.toString().c_str(),view.toString().c_str());
    shaderProgram.setUniformMatrix("u_proj",proj);
    shaderProgram.setUniformMatrix("u_view",view);
            
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

 void pause(){}
 void resume(){}
 void dispose(){}
};

int main(int argc, char* argv[])
{
    LibGDX_Application* app = new LibGDX_Application(new MyAppListener());
    return 0;
}
