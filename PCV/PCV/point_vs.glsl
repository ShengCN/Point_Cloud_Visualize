#version 400            
uniform mat4 PVM;
uniform float time;

in vec3 pos_attrib;
in vec3 col_attrib;

out vec3 vs_col;

void main()
{
   gl_Position = PVM * vec4(pos_attrib, 1.0);
   vs_col = col_attrib;
}