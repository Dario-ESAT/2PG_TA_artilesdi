#version 330



layout (location=0) in vec3 a_position;
layout (location=1) in vec3 a_normal;

float pi = 3.141592653589793;

vec3 scalation = vec3(1.0,1.0,1.0);
mat4 scale = mat4(vec4(scalation.x,0.0,0.0,0.0),
                  vec4(0.0,scalation.y,0.0,0.0),
                  vec4(0.0,0.0,scalation.z,0.0),
                  vec4(0.0,0.0,0.0,1.0));

float radx = 0.4;
mat4 rotatex = mat4(vec4(1.0,0.0,0.0,0.0),
                  vec4(0.0,cos(radx),sin(radx),0.0),
                  vec4(0.0,-sin(radx),cos(radx),0.0),
                  vec4(0.0,0.0,0.0,1.0));

float rady = pi / 4;
mat4 rotatey = mat4(vec4(cos(rady),0.0,sin(rady),0.0),
                    vec4(0.0,1.0,0.0,0.0),
                    vec4(-sin(rady),0.0,cos(rady),0.0),
                    vec4(0.0,0.0,0.0,1.0));

float radz = 0;
mat4 rotatez = mat4(vec4(cos(radz),sin(radz),0.0,0.0),
                    vec4(-sin(radz),cos(radz),0.0,0.0),
                    vec4(0.0,0.0,1.0,0.0),
                    vec4(0.0,0.0,0.0,1.0));


vec3 trans = vec3(0.0,0.0,0.0);
mat4 translate = mat4(vec4(1.0,0.0,0.0,0.0),
                      vec4(0.0,1.0,0.0,0.0),
                      vec4(0.0,0.0,1.0,0.0),
                      vec4(trans.x,trans.y,trans.z,1.0));

mat4 modelMatrix = rotatex * rotatey * rotatez * scale * translate;

uniform mat4 u_m_matrix;
uniform mat4 u_vp_matrix;

out vec3 normal;

void main()
{
	gl_Position = u_vp_matrix * modelMatrix * /* u_m_matrix  * */ vec4(a_position, 1.0);
}
