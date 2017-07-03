#pragma once

#include "GL.h"
#include "graphics/Mesh.h"
#include <vector>
#include "graphics/VertexAttribute.h"

class MeshBuilder
{
public:
    std::vector<Mesh> meshes;
    float degreesToRadians = M_PI / 180.0f;
    
    MeshBuilder(){
        meshes = std::vector<Mesh>();
    }

    void build(float width, float height,float depth, int divisionsU,int divisionsV){
        build(width,height,depth,divisionsU,divisionsV,0,360,0,180);
    }

    void build(float width, float height, float depth,
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
        std::vector<GLuint> tmpIndices(s);
        int tempOffset = 0;
        
        std::vector<Vector3> vertices;
        std::vector<GLfloat> vertexValues;
        std::vector<GLuint> indices;

        for (int iv = 0; iv <= divisionsV; iv++) {
            angleV = avo + stepV * iv;
            float t = sin(angleV);
            float h = cos(angleV) * hh;
            for (int iu = 0; iu <= divisionsU; iu++) {
                angleU = auo + stepU * iu;
                // Fixme : wrong normal calculation if transform
                //SDL_Log("NEW VERTEX: %s",Vector3(cos(angleU) * hw * t, h, sin(angleU) * hd * t).add(mesh.transform).toString().c_str());
                //Vector3 tempVector = Vector3(cos(angleU) * hw * t, h, sin(angleU) * hd * t).add(mesh.transform);
                vertices.push_back(Vector3(cos(angleU) * hw * t, h, sin(angleU) * hd * t));
                tmpIndices[tempOffset] = vertices.size() - 1;
                int o = tempOffset + s;
                if ((iv > 0) && (iu > 0)) // FIXME don't duplicate lines and points
                {
                    //SDL_Log("NEW INDICES: %i,%i,%i,%i,%i,%i",
                    //tmpIndices[tempOffset],tmpIndices[(o - 1) % s],tmpIndices[(o - (divisionsU + 2)) % s],
                    //tmpIndices[(o - (divisionsU + 2)) % s],tmpIndices[(o - (divisionsU + 1)) % s],tmpIndices[tempOffset]);
                    indices.push_back(tmpIndices[tempOffset]);                 //corner00 = tmpIndices.get(tempOffset)
                    indices.push_back(tmpIndices[(o - 1) % s]);                //corner10 = tmpIndices.get((o - 1) % s)
                    indices.push_back(tmpIndices[(o - (divisionsU + 2)) % s]); //corner11 = tmpIndices.get((o - (divisionsU + 2)) % s)
                    indices.push_back(tmpIndices[(o - (divisionsU + 2)) % s]); //corner11 = tmpIndices.get((o - (divisionsU + 2)) % s)
                    indices.push_back(tmpIndices[(o - (divisionsU + 1)) % s]); //corner01 = tmpIndices.get((o - (divisionsU + 1)) % s))
                    indices.push_back(tmpIndices[tempOffset]);                 //corner00 = tmpIndices.get(tempOffset)
                }
                tempOffset = (tempOffset + 1) % s;
            }
        }
        
        for(Vector3 vec3:vertices){
            vertexValues.push_back(vec3.x);
            vertexValues.push_back(vec3.y);
            vertexValues.push_back(vec3.z);
        }
        
        std::vector<VertexAttribute> attr{VertexAttribute::position()};
        meshes.push_back(Mesh(VertexData(VERTEX_BUFFER_OBJECT,true,vertexValues.size(),attr),
                        IndexData(INDEX_BUFFER_OBJECT,true,indices.size()),true));
        meshes[meshes.size()-1].setVertices(vertexValues);
        meshes[meshes.size()-1].setIndices(indices);
    }
};
