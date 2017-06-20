in vec3 a_position;
in vec4 a_color;

//uniform mat4 u_proj,u_view;
uniform mat4 u_projView;

void main()
{
    //v_color = a_color;
    //v_color.a = v_color.a * (256.0/255.0);
    //v_texCoords = a_texCoord + 0;

    gl_Position =  u_projView * vec4(a_position,1);
    //gl_Position =  u_proj * u_view * vec4(a_position,1);
}
