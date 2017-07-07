in vec3 a_position;

in vec4 a_color;
out vec4 v_color;

in vec2 a_texCoord0;
out vec2 v_texCoord0;

uniform mat4 u_projView;

void main()
{
    v_color = a_color;
    v_texCoord0 = a_texCoord0;
    
    gl_Position = u_projView * vec4(a_position,1);
}
