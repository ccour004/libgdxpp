#pragma once

#ifdef DESKTOP
	#include <GL/glew.h> 
    #include <SDL_opengl.h> 
	#include <GL/glu.h>
#else
	#include <GLES3/gl3.h>
#endif

#include <SDL.h>
#include <sstream>
//#include "math/Vector3.h"

char* file_read(const char* filename) {
        SDL_RWops *rw = SDL_RWFromFile(filename, "rb");
        if (rw == NULL) return NULL;

        Sint64 res_size = SDL_RWsize(rw);
        char* res = (char*)malloc(res_size + 1);

        Sint64 nb_read_total = 0, nb_read = 1;
        char* buf = res;
        while (nb_read_total < res_size && nb_read != 0) {
                nb_read = SDL_RWread(rw, buf, 1, (res_size - nb_read_total));
                nb_read_total += nb_read;
                buf += nb_read;
        }
        SDL_RWclose(rw);
        if (nb_read_total != res_size) {
                free(res);
                return NULL;
        }

        res[nb_read_total] = '\0';
        return res;
}

std::string fileToString(const std::string& filename){
    return std::string(file_read(filename.c_str()));
}

/*class Vector2{
public:
    Vector2(float x,float y){this->x = x;this->y = y;}
    float x,y;
};

class Vector3{
public:
    Vector3(){};
    Vector3(float x,float y,float z){this->x = x;this->y = y;this->z = z;}
    float x,y,z;
    Vector3 nor(){
        float magnitude = sqrtf((x * x) + (y * y) + (z * z));
        x /= magnitude;y /= magnitude;z /= magnitude;
        return Vector3(x,y,z);
    }
    Vector3 scl(float val){
        x *= val;y *= val;z *= val;
        return Vector3(x,y,z);
    }
    static Vector3 cross(const Vector3& a,const Vector3& b){
        return Vector3((a.y * b.z) - (a.z * b.y),
                       (a.z * b.x) - (a.x * b.z),
                       (a.x * b.y) - (a.y * b.x));
    }
    static float dot(const Vector3& a,const Vector3& b){
        return (a.x*b.x)+(a.y*b.y)+(a.z*b.z);
    }
    Vector3 operator -(const Vector3&a){
        return Vector3(x-a.x,y-a.y,z-a.z);
    }
    std::string toString(){
            std::stringstream toReturn;
            toReturn << "{"<<x<<","<<y<<","<<z << "}";
            return toReturn.str();
        }
};

class Vector4{
public:
    Vector4(){};
    Vector4(float x,float y,float z,float w){this->x = x;this->y = y;this->z = z;this->w = w;}
    float x,y,z,w;
    float operator *(const Vector4& a){
        return x*a.x+y*a.y+z*a.z+w*a.w;
    }
};

class Matrix{
    protected:
        float* data;
        int size,rowSize;
    public:
        float* getData(){return data;}
        Matrix(){}
        Matrix(const Matrix& other){
            size = other.size;
            rowSize = other.rowSize;
            data = new float[size];
            for(int i = 0;i < size;i++) data[i] = other.data[i];
        }
        ~Matrix(){delete data;}
        std::string toString(){
            std::stringstream toReturn;
            toReturn << "{";
            for(int i = 0;i < size;i++){
                toReturn << data[i];
                if(i != size - 1) toReturn << ",";
                if(i != 0 && i != size-1 && (i+1) % rowSize == 0) toReturn << "\n\t";
            }
            toReturn << "}";
            return toReturn.str();
        }
};

class Matrix3:public Matrix{
public:
    Matrix3(){
        data = new float[9]{1,0,0,
                           0,1,0,
                           0,0,1};
    }
};

class Matrix4:public Matrix{
public:
    Matrix4(float in[16]){
        data = in;
        rowSize = 4;
        size = 16;
    }
    Matrix4(Vector4 row1,Vector4 row2,Vector4 row3,Vector4 row4){
        data = new float[16];
        data[0] = row1.x;data[1] = row1.y;data[2] = row1.z;data[3] = row1.w;
        data[4] = row2.x;data[5] = row2.y;data[6] = row2.z;data[7] = row2.w;
        data[8] = row3.x;data[9] = row3.y;data[10] = row3.z;data[11] = row3.w;
        data[12] = row4.x;data[13] = row4.y;data[14] = row4.z;data[15] = row4.w;
    }
    Matrix4(){
        data = new float[16]{
            1,0,0,0,
            0,1,0,0,
            0,0,1,0,
            0,0,0,1
        };
    }
    static Matrix4 mul(Matrix4& a,Matrix4& b){
        Vector4 rows[4],cols[4];
        for(int i = 0;i < 16;i+=4){
            rows[i / 4] = Vector4(a.getData()[i],a.getData()[i+1],a.getData()[i+2],a.getData()[i+3]);
            cols[i / 4] = Vector4(b.getData()[i],b.getData()[i+4],b.getData()[i+8],b.getData()[i+12]);
        }
        return Matrix4(new float[16]{
            rows[0]*cols[0],rows[0]*cols[1],rows[0]*cols[2],rows[0]*cols[3],
            rows[1]*cols[0],rows[1]*cols[1],rows[1]*cols[2],rows[1]*cols[3],
            rows[2]*cols[0],rows[2]*cols[1],rows[2]*cols[2],rows[2]*cols[3],
            rows[3]*cols[0],rows[3]*cols[1],rows[3]*cols[2],rows[3]*cols[3]
        });       
    }
};

Matrix4 operator *(const Matrix4& a,const Matrix4& b){
    Vector4 rows[4],cols[4];
    for(int i = 0;i < 16;i+=4){
        rows[i / 4] = Vector4(a.getData()[i],a.getData()[i+1],a.getData()[i+2],a.getData()[i+3]);
        cols[i / 4] = Vector4(b.getData()[i],b.getData()[i+4],b.getData()[i+8],b.getData()[i+12]);
    }
    return Matrix4(new float[16]{
        rows[0]*cols[0],rows[0]*cols[1],rows[0]*cols[2],rows[0]*cols[3],
        rows[1]*cols[0],rows[1]*cols[1],rows[1]*cols[2],rows[1]*cols[3],
        rows[2]*cols[0],rows[2]*cols[1],rows[2]*cols[2],rows[2]*cols[3],
        rows[3]*cols[0],rows[3]*cols[1],rows[3]*cols[2],rows[3]*cols[3]
    });
}*/

class Color{
public:
    Color(float r,float g,float b,float a){this->r = r;this->g = g;this->b = b;this->a = a;}
    float r,g,b,a;
};

/*class Ray{
public:
    Vector3 origin,direction;
    Ray(Vector3 origin,Vector3 direction){
        this->origin = origin;
        this->direction = direction;
    }
};*/

class Frustum{
  public:
Frustum(){}  
};

