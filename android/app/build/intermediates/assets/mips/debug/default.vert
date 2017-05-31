#version 300 es
#define M_PI 3.1415926535897932384626433832795f

in vec3 position;

uniform mat4 m_model;
uniform mat4 m_view;
uniform mat4 m_projection;

uniform vec3 eye;
uniform vec3 center;
uniform vec3 up;

uniform vec4 view;

void main() {
    //PROJ
    float near = /*0.1f*/view.x,far = /*100.0f*/view.y,fov = /*67.0f*/view.z,
        aspect = /*0.6182f*/view.w;
    float top = near * tan(M_PI / 180.0f * fov / 2.0f), bottom = -top,
        right = top * aspect, left = -right;
    mat4 proj = mat4(
        (2.0f * near) / (right-left),0,(right+left)/(right-left),0,
        0,(2.0f * near)/(top-bottom),(top+bottom)/(top-bottom),0,
        0,0,-1.0f * ((far+near)/(far-near)),-(2.0f * far*near) / (far-near),
        0,0,-1,1
        );

    //VIEW
    //vec3 eye = vec3(0,0,3.0f);
    //vec3 center = vec3(0,0,0);
    //vec3 up = vec3(0,1.0f,0);
    vec3 zaxis = normalize(eye-center);
    vec3 xaxis = normalize(cross(up,zaxis));
    vec3 yaxis = cross(zaxis,xaxis);
    mat4 view = mat4(
        xaxis.x,yaxis.x,zaxis.x,0,
        xaxis.y,yaxis.y,zaxis.y,0,
        xaxis.z,yaxis.z,zaxis.z,0,
        -dot(xaxis,eye),-dot(yaxis,eye),-dot(zaxis,eye),1
    );
    gl_Position = /*m_projection * m_view * m_model */ proj * view * vec4(position,1);
}