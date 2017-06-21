#ifdef GL_ES
    //#define LOWP lowp
    precision mediump float;
#else
    #define LOWP
#endif

//in LOWP vec4 v_color;
out vec4 out_color;

void main()
{
    out_color = /*v_color*/vec4( 1.0, 1.0, 1.0, 1.0 );
}
