#version 330

layout (location=0) in vec3 a_position;
float pi = 3.141592653589793;
mat4 identity = mat4(vec4(1.0,0.0,0.0,0.0),
                      vec4(0.0,1.0,0.0,0.0),
                      vec4(0.0,0.0,1.0,0.0),
                      vec4(0.0,0.0,0.0,1.0));


vec3 scalation = vec3(1.0,1.0,1.0);
mat4 scale = mat4(vec4(scalation.x,0.0,0.0,0.0),
                  vec4(0.0,scalation.y,0.0,0.0),
                  vec4(0.0,0.0,scalation.z,0.0),
                  vec4(0.0,0.0,0.0,1.0));


float radx = 0.0;
mat4 rotatex = mat4(vec4(1.0,0.0,0.0,0.0),
                  vec4(0.0,cos(radx),sin(radx),0.0),
                  vec4(0.0,-sin(radx),cos(radx),0.0),
                  vec4(0.0,0.0,0.0,1.0));


float rady = -1.0;
mat4 rotatey = mat4(vec4(cos(rady),0.0,sin(rady),0.0),
                    vec4(0.0,1.0,0.0,0.0),
                    vec4(-sin(rady),0.0,cos(rady),0.0),
                    vec4(0.0,0.0,0.0,1.0));


float radz = 0.0;
mat4 rotatez = mat4(vec4(cos(radz),sin(radz),0.0,0.0),
                    vec4(-sin(radz),cos(radz),0.0,0.0),
                    vec4(0.0,0.0,1.0,0.0),
                    vec4(0.0,0.0,0.0,1.0));


vec3 trans = vec3(0.00,00.00,0.0);
mat4 translate = mat4(vec4(1.0,0.0,0.0,0.0),
                      vec4(0.0,1.0,0.0,0.0),
                      vec4(0.0,0.0,1.0,0.0),
                      vec4(trans.x,trans.y,trans.z,1.0));

float angleOfView = 40.0;
float near = 0.001;
float far = 100.0;

float scaleView = 1 / tan(angleOfView * 0.5 * pi / 180);

mat4 proyectionmatrix = mat4(vec4(scaleView,0.0, 0.0, 0.0),
                      vec4(0.0  ,scaleView,0.0 , 0.0),
                      vec4(0.0  ,0.0  ,-far / (far - near),-1),
                      vec4(0.0  ,0.0  ,-far * near / (far - near) ,1.0));


void main()
{
  gl_Position = proyectionmatrix * rotatex * rotatey * rotatez * scale * translate * vec4(a_position, 500);
}