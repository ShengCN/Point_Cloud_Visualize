#version 400

uniform int iterations;
uniform int int_slider;
uniform int w;
uniform int h;

in vec3 vs_col;
out vec4 fragcolor;           

void main(void)
{
   // if(length(gl_PointCoord - gl_FragCoord.xy)>int_slider)
   // {
   //    fragcolor = vec4(0.0, sin(float(iterations) * 0.1), 1.0, 0.5);
   // }
   // else
   //    fragcolor = vec4(1.0, sin(float(iterations) * 0.1), 0.0, 0.5);

   if(length(vs_col) < 0.01f){
      fragcolor = vec4(0.7f, 0.3f, 0.0f, 0.0f);
   }
   else{
      fragcolor = vec4(vs_col, 1.0f);
   }
}