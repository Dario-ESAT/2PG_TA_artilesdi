#version 330

layout (location=0) in vec3 a_position;
float pi = 3.141592653589793;

float rady = pi / 2;
mat4 rotatey = mat4(vec4(cos(rady),0.0,sin(rady),0.0),
                    vec4(0.0,1.0,0.0,0.0),
                    vec4(-sin(rady),0.0,cos(rady),0.0),
                    vec4(0.0,0.0,0.0,1.0));


vec3 trans = vec3(0.00,00.00,-0.80);
mat4 translate = mat4(vec4(1.0,0.0,0.0,0.0),
                      vec4(0.0,1.0,0.0,0.0),
                      vec4(0.0,0.0,1.0,0.0),
                      vec4(trans.x,trans.y,trans.z,1.0));

void main()
{
  gl_Position = rotatey * translate * vec4(a_position, 500);
}