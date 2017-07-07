#ifdef GL_ES
    #define LOWP lowp
    precision mediump float;
#else
    #define LOWP
#endif

in LOWP vec4 v_color;
in vec2 v_texCoord0;
out vec4 out_color;

uniform sampler2D tex;

void main()
{
    out_color = texture(tex, v_texCoord0)/* * v_color*/;
}
