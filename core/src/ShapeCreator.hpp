#ifndef SDL_H
#define SDL_H
#include <SDL.h>
#endif

#include <LinearMath/btVector3.h>
#include <LinearMath/btHashMap.h>
#include <math.h>
#include <sstream>
#include <fstream>

#ifdef DESKTOP
	#include <GL/glew.h> 
        #include <SDL_opengl.h> 
	#include <GL/glu.h>
#else
	#include <GLES3/gl3.h>
#endif
//#include <SDL_rwops.h>

using namespace std;

struct Shader{
    GLuint gProgramID = 0;
public:
    GLint positionLoc = -1,modelLoc = -1,viewLoc = -1,projLoc = -1;
    GLint eyeLoc = -1,centerLoc = -1,upLoc = -1;
    float* modelMatrix,*viewMatrix,*projMatrix;
    float eye[3] = {0,0,3.0f},center[3] = {0,0,0},up[3] = {0,1,0};
    float aspectRatio = 0.6182f;

    bool setModelMatrix(float x,float y,float z){
        modelMatrix = new float[16];
        for(int i = 0;i < 16;i++)
            switch(i){
                case 0:case 5:case 10:case 15: modelMatrix[i] = 1.0f; break;
                case 3: modelMatrix[i] = x;break;
                case 7: modelMatrix[i] = y;break;
                case 11: modelMatrix[i] = z;break;
            }
        return true;
    }

    bool setProjectionMatrix(float near,float far,float aspect,float fov){
        projMatrix = new float[16];
        float top = near * tan((M_PI*180.0f)*(fov/2.0f)),bottom = -top,
                right = top * aspect,left = -right;
        for(int i = 0;i < 16;i++)
            switch(i){
                case 0:projMatrix[i] = (2.0f * near) / (right - left);break;
                case 2:projMatrix[i] = (right+left) / (right-left);break;
                case 5:projMatrix[i] = (2.0f*near) / (top-bottom);break;
                case 6:projMatrix[i] = (top+bottom) / (top-bottom);break;
                case 10:projMatrix[i] = -(far+near) / (far-near);break;
                case 11:projMatrix[i] = -(2.0f * far * near) / (far-near);break;
                case 14:projMatrix[i] = -1.0f;break;
                case 15:projMatrix[i] = 1.0f;break;
                default:projMatrix[i] = 0.0f;
            }
        return true;
    }

    bool setViewMatrix(btVector3 eye,btVector3 target,btVector3 up){
        btVector3 zaxis = btVector3(eye - target).normalized();
        btVector3 xaxis = up.cross(zaxis).normalized();
        btVector3 yaxis = zaxis.cross(xaxis);

        // Create a 4x4 view matrix from the right, up, forward and eye position vectors
        viewMatrix = new float[16]{xaxis.x(),yaxis.x(),zaxis.x(),0,
                                   xaxis.y(),yaxis.y(),zaxis.y(),0,
                                   xaxis.z(),yaxis.z(),zaxis.z(),0,
                                   -xaxis.dot(eye),-yaxis.dot(eye),-zaxis.dot(eye), 1};
        return true;
    }
    GLuint getProgramID(){return gProgramID;}
    bool init() {
        //Generate program
        gProgramID = glCreateProgram();
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

        //Load shader from assets folder.
            long size = 1500;
            char* buffer = (char*) malloc (sizeof(char)*size);
	    #ifdef DESKTOP
	    string filename = "android/app/src/main/assets/default.vert";
	    #else
	    string filename = "default.vert";
	    #endif
	    SDL_RWops *rw=SDL_RWFromFile(filename.c_str(),"rb");
	    if(rw==NULL) {
	      fprintf(stderr,"Couldn't open default.vert\n");
	      return(1);
	    }

	    int blocks=SDL_RWread(rw,buffer,16,size/16);
	    SDL_RWclose(rw);
	    if(blocks<0) {
	      fprintf(stderr,"Couldn't read from default.vert\n");
	      return(2);
	    }

        //Compile shader source.
        string vs(buffer);
        size_t pos = vs.find_last_of("}");
        vs = vs.substr(0,pos+1);
        const GLchar* ob1 = vs.c_str();
        glShaderSource(vertexShader, 1, &ob1, NULL);
        glCompileShader(vertexShader);

        //Check vertex shader for errors
        GLint vShaderCompiled = GL_FALSE;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
        if (vShaderCompiled != GL_TRUE) {
            SDL_Log("Unable to compile vertex shader %d!\n", vertexShader);
            printShaderLog(vertexShader);
            return false;
        } else {
            //Attach vertex shader to program
            glAttachShader(gProgramID, vertexShader);

            //Create fragment shader
            GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        //Load shader from assets folder.
            long size = 1500;
            char* buffer = (char*) malloc (sizeof(char)*size);
	    #ifdef DESKTOP
	    string filename = "android/app/src/main/assets/default.frag";
	    #else
	    string filename = "default.frag";
	    #endif
	    SDL_RWops *rw=SDL_RWFromFile(filename.c_str(),"rb");
	    if(rw==NULL) {
	      fprintf(stderr,"Couldn't open default.frag\n");
	      return(1);
	    }

	    int blocks=SDL_RWread(rw,buffer,16,size/16);
	    SDL_RWclose(rw);
	    if(blocks<0) {
	      fprintf(stderr,"Couldn't read from default.frag\n");
	      return(2);
	    }

            //Compile shader source.
            string vs(buffer);
            size_t pos = vs.find_last_of("}");
            vs = vs.substr(0,pos+1);
            const GLchar* ob2 = vs.c_str();
            glShaderSource(fragmentShader, 1, &ob2, NULL);
            glCompileShader(fragmentShader);

            //Check fragment shader for errors
            GLint fShaderCompiled = GL_FALSE;
            glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
            if (fShaderCompiled != GL_TRUE) {
                SDL_Log("Unable to compile fragment shader %d!\n", fragmentShader);
                printShaderLog(fragmentShader);
                return false;
            } else {
                //Attach fragment shader to program
                glAttachShader(gProgramID, fragmentShader);

                //Link program
                glLinkProgram(gProgramID);

                //Check for errors
                GLint programSuccess = GL_TRUE;
                glGetProgramiv(gProgramID, GL_LINK_STATUS, &programSuccess);
                if (programSuccess != GL_TRUE) {
                    SDL_Log("Error linking program %d!\n", gProgramID);
                    printProgramLog(gProgramID);
                    return false;
                }else{
                    positionLoc = glGetAttribLocation(gProgramID, "position" );
                    if(positionLoc == -1 )
                    {
                        SDL_Log( "position is not a valid glsl program variable!\n" );
                        return false;
                    }

                    projLoc = glGetUniformLocation(gProgramID, "m_projection");
                    if(projLoc == -1 )
                    {
                        SDL_Log( "m_projection is not a valid glsl program variable!\n" );
                        //return false;
                    }

                    viewLoc = glGetUniformLocation(gProgramID, "m_view");
                    if(viewLoc == -1 )
                    {
                        SDL_Log( "m_view is not a valid glsl program variable!\n" );
                        //return false;
                    }

                    modelLoc = glGetUniformLocation(gProgramID, "m_model");
                    if(modelLoc == -1 )
                    {
                        SDL_Log( "m_model is not a valid glsl program variable!\n" );
                        //return false;
                    }

                    eyeLoc = glGetUniformLocation(gProgramID,"eye");
                    centerLoc = glGetUniformLocation(gProgramID,"center");
                    upLoc = glGetUniformLocation(gProgramID,"up");
                }
            }
        }
        return true;
    }

    void printShaderLog( GLuint shader )
    {
        //Make sure name is shader
        if( glIsShader( shader ) )
        {
            //Shader log length
            int infoLogLength = 0;
            int maxLength = infoLogLength;

            //Get info string length
            glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &maxLength );

            //Allocate string
            char* infoLog = new char[ maxLength ];

            //Get info log
            glGetShaderInfoLog( shader, maxLength, &infoLogLength, infoLog );
            if( infoLogLength > 0 )
            {
                //Print Log
                SDL_Log( "%s\n", infoLog );
            }

            //Deallocate string
            delete[] infoLog;
        }
        else
        {
            SDL_Log( "Name %d is not a shader\n", shader );
        }
    }

    void printProgramLog( GLuint program )
    {
        //Make sure name is shader
        if( glIsProgram( program ) )
        {
            //Program log length
            int infoLogLength = 0;
            int maxLength = infoLogLength;

            //Get info string length
            glGetProgramiv( program, GL_INFO_LOG_LENGTH, &maxLength );

            //Allocate string
            char* infoLog = new char[ maxLength ];

            //Get info log
            glGetProgramInfoLog( program, maxLength, &infoLogLength, infoLog );
            if( infoLogLength > 0 )
            {
                //Print Log
                SDL_Log( "%s\n", infoLog );
            }

            //Deallocate string
            delete[] infoLog;
        }
        else
        {
            SDL_Log( "Name %d is not a program\n", program );
        }
    }
};

struct Mesh {
    btVector3 transform;
    btAlignedObjectArray<GLuint> indices;
    btAlignedObjectArray<btVector3> vertices;
    GLuint* indicesGL;
    GLfloat* verticesGL;
    GLuint gVBO = 0,gIBO = 0;
public:
    ~Mesh(){delete indicesGL; delete verticesGL;}
    void init(){
        transform = btVector3(0,0,0);
        //VBO/IBO data
        getVertices();
        getIndices();

        //Create VBO
        glGenBuffers( 1, &gVBO );
        glBindBuffer( GL_ARRAY_BUFFER, gVBO );
        glBufferData( GL_ARRAY_BUFFER, 3 * vertices.size() * sizeof(GLfloat), verticesGL, GL_STATIC_DRAW );

        //Create IBO
        glGenBuffers( 1, &gIBO );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gIBO );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indicesGL, GL_STATIC_DRAW );
    }

    void render(){
        //Set index data and render
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gIBO );
        glDrawElements( GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL );
    }

    void getIndices(){
        if(indicesGL == NULL){
            indicesGL = new GLuint[indices.size()];
            for(int i = 0;i < indices.size();i++) indicesGL[i] = indices.at(i);
        }
    }

    void getVertices(){
        if(verticesGL == NULL){
            verticesGL = new GLfloat[vertices.size() * 3];
            int index = 0;
            for(int i = 0;i < vertices.size();i++){
                verticesGL[index++] = vertices[i].x();
                verticesGL[index++] = vertices[i].y();
                verticesGL[index++] = vertices[i].z();
            }
        }
    }

    void printIndices(){
        for(int i = 0;i < indices.size();i += 3){
            SDL_Log("FACE %i: {%i,%i,%i}",i/3,indices[i],indices[i+1],indices[i+2]);
        }
    }

    void printVertices(){
        SDL_Log("PRINT VERT START: %i",vertices.size());
        for(int i = 0;i < vertices.size();i++){
            SDL_Log("VERT %i: {%f,%f,%f}",i,vertices[i].x(),vertices[i].y(),vertices[i].z());
        }
    }
};

struct MeshInstance{
    Mesh* mesh;
    Shader* shader;
    btVector3 transform;
public:
    MeshInstance(Mesh* mesh,Shader* shader,btVector3 transform){
        this->mesh = mesh;
        this->shader = shader;
        this->transform = transform;
    }

    void resize(int width,int height){
        shader->aspectRatio = ((float)width)/((float)height);
    }

    void render(){
            //Bind program
            glUseProgram(shader->getProgramID());

            //Set transform
            glUniform3f(glGetUniformLocation(shader->getProgramID(),"eye"),shader->eye[0],shader->eye[1],shader->eye[2]);
            glUniform3f(glGetUniformLocation(shader->getProgramID(),"center"),shader->center[0],shader->center[1],shader->center[2]);
            glUniform3f(glGetUniformLocation(shader->getProgramID(),"up"),shader->up[0],shader->up[1],shader->up[2]);
            glUniform4f(glGetUniformLocation(shader->getProgramID(),"view"),0.1f,100.0f,67.0f,shader->aspectRatio);

            //Enable vertex position
            glEnableVertexAttribArray(shader->positionLoc);

            //Set vertex data
            glBindBuffer( GL_ARRAY_BUFFER, mesh->gVBO );
            glVertexAttribPointer(shader->positionLoc, 3, GL_FLOAT, GL_FALSE,3 * sizeof(GLfloat), NULL );

            //Render mesh one time.
            mesh->render();

            //Disable vertex position
            glDisableVertexAttribArray(shader->positionLoc);

            //Unbind program
            glUseProgram(NULL);
    }
};

class MeshBuilder
{
public:
    btAlignedObjectArray<Mesh> meshes;Mesh mesh;

    float degreesToRadians = 3.1415927f / 180.0f;

    Mesh build(float width, float height,float depth, int divisionsU,int divisionsV){
        return build(width,height,depth,divisionsU,divisionsV,0,360,0,180);
    }

    Mesh build(float width, float height, float depth,
                          int divisionsU, int divisionsV, float angleUFrom, float angleUTo, float angleVFrom, float angleVTo) {
        //Mesh mesh;

        float hw = width * 0.5f;
        float hh = height * 0.5f;
        float hd = depth * 0.5f;
        float auo = degreesToRadians * angleUFrom;
        float stepU = (degreesToRadians * (angleUTo - angleUFrom)) / divisionsU;
        float avo = degreesToRadians * angleVFrom;
        float stepV = (degreesToRadians * (angleVTo - angleVFrom)) / divisionsV;
        float us = 1.0f / divisionsU;
        float vs = 1.0f / divisionsV;
        float u = 0.0f;
        float v = 0.0f;
        float angleU = 0.0f;
        float angleV = 0.0f;

        int s = divisionsU + 3;
        GLuint tmpIndices[s];
        int tempOffset = 0;

        for (int iv = 0; iv <= divisionsV; iv++) {
            angleV = avo + stepV * iv;
            float t = sin(angleV);
            float h = cos(angleV) * hh;
            for (int iu = 0; iu <= divisionsU; iu++) {
                angleU = auo + stepU * iu;
                // Fixme : wrong normal calculation if transform
                mesh.vertices.push_back(btVector3(cos(angleU) * hw * t, h, sin(angleU) * hd * t)/* * mesh.transform*/);
                tmpIndices[tempOffset] = mesh.vertices.size()-1;
                int o = tempOffset + s;
                if ((iv > 0) && (iu > 0)) // FIXME don't duplicate lines and points
                {
                    mesh.indices.push_back(tmpIndices[tempOffset]);                 //corner00 = tmpIndices.get(tempOffset)
                    mesh.indices.push_back(tmpIndices[(o - 1) % s]);                //corner10 = tmpIndices.get((o - 1) % s)
                    mesh.indices.push_back(tmpIndices[(o - (divisionsU + 2)) % s]); //corner11 = tmpIndices.get((o - (divisionsU + 2)) % s)
                    mesh.indices.push_back(tmpIndices[(o - (divisionsU + 2)) % s]); //corner11 = tmpIndices.get((o - (divisionsU + 2)) % s)
                    mesh.indices.push_back(tmpIndices[(o - (divisionsU + 1)) % s]); //corner01 = tmpIndices.get((o - (divisionsU + 1)) % s))
                    mesh.indices.push_back(tmpIndices[tempOffset]);                 //corner00 = tmpIndices.get(tempOffset)
                }
                tempOffset = (tempOffset + 1) % s;
            }
        }

        meshes.push_back(mesh);
        return meshes.at(meshes.size()-1);
    }
};
