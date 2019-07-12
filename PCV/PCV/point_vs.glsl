#version 400            
uniform mat4 PVM;
uniform float time;

in vec3 pos_attrib;


void main()
{
   gl_Position = PVM*vec4(pos_attrib, 1.0);
}