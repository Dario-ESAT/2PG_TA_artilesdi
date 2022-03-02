#version 330

layout (location=0) in vec3 a_position;
float pi = 3.141592653589793;
vec3 scalation = vec3(5.0,5.0,1.0);
mat4 scale = mat4(vec4(scalation.x,0.0,0.0,0.0),
                  vec4(0.0,scalation.y,0.0,0.0),
                  vec4(0.0,0.0,scalation.z,0.0),
                  vec4(0.0,0.0,0.0,1.0));
void main()
{
  gl_Position = scale * vec4(a_position, 500);
}