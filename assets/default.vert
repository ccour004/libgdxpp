in vec3 a_position;
//in vec4 a_color;

uniform mat4 u_projView;

void main()
{
    //v_color = a_color;
    gl_Position =  u_projView * vec4(a_position,1);
}
