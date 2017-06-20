#pragma once

#include "GL.h"
#include <vector>
#include <math.h>
#include <sstream>
#include <fstream>

struct Mesh {
    Vector3 transform = Vector3(0,0,0);
    std::vector<GLuint> indices;
    std::vector<Vector3> vertices;
    GLuint* indicesGL;
    GLfloat* verticesGL;
    GLuint gVBO = 0,gIBO = 0;
public:
    ~Mesh(){delete indicesGL; delete verticesGL;}
    void init(){
        transform = Vector3(0,0,0);
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
                verticesGL[index++] = vertices[i].x;
                verticesGL[index++] = vertices[i].y;
                verticesGL[index++] = vertices[i].z;
            }
        }
    }

    void printIndices(){
        for(int i = 0;i < indices.size();i += 3){
            SDL_Log("FACE %i: {%i,%i,%i}",i/3,indices[i],indices[i+1],indices[i+2]);
        }
    }

    void printVertices(){
        SDL_Log("PRINT VERT START: %lu",vertices.size());
        for(int i = 0;i < vertices.size();i++){
            SDL_Log("VERT %i: {%f,%f,%f}",i,vertices[i].x,vertices[i].y,vertices[i].z);
        }
    }
};

class MeshBuilder
{
public:
    std::vector<Mesh> meshes;Mesh mesh;
    float degreesToRadians = 3.1415927f / 180.0f;

    Mesh build(float width, float height,float depth, int divisionsU,int divisionsV){
        return build(width,height,depth,divisionsU,divisionsV,0,360,0,180);
    }

    Mesh build(float width, float height, float depth,
                          int divisionsU, int divisionsV, float angleUFrom, float angleUTo, float angleVFrom, float angleVTo) {
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
                mesh.vertices.push_back(Vector3(cos(angleU) * hw * t, h, sin(angleU) * hd * t).add(mesh.transform));
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
