#version 330

layout (location=0) in vec3 a_position;
float pi = 3.141592653589793;

float radx = pi;
mat4 rotatex = mat4(vec4(1.0,0.0,0.0,0.0),
                  vec4(0.0,cos(radx),sin(radx),0.0),
                  vec4(0.0,-sin(radx),cos(radx),0.0),
                  vec4(0.0,0.0,0.0,1.0));
void main()
{
  gl_Position = rotatex * vec4(a_position, 500);
}