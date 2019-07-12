#version 400

uniform int iterations;
uniform int int_slider;
uniform int w;
uniform int h;

in vec2 gl_PointCoord;

in vec4 gl_FragCoord;

out vec4 fragcolor;           

void main(void)
{
   // if(length(gl_PointCoord - gl_FragCoord.xy)>int_slider)
   // {
   //    fragcolor = vec4(0.0, sin(float(iterations) * 0.1), 1.0, 0.5);
   // }
   // else
   //    fragcolor = vec4(1.0, sin(float(iterations) * 0.1), 0.0, 0.5);

   fragcolor = vec4(1.0, sin(float(iterations) * 0.1), 0.0, 0.5);
}