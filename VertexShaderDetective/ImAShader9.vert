#version 330

layout (location=0) in vec3 a_position;
float pi = 3.141592653589793;

float angleOfView = 60.0;
float near = 0.001;
float far = 100.0;

float scaleView = 1 / tan(angleOfView * 0.5 * pi / 180);

mat4 proyectMatrix = mat4(vec4(scaleView,0.0      ,0.0                        ,0.0),
                          vec4(0.0      ,scaleView,0.0                        ,0.0),
                          vec4(0.0      ,0.0      ,-far / (far - near)        ,-1),
                          vec4(0.0      ,0.0      ,-far * near / (far - near) ,1.0));



void main() {

  gl_Position = proyectMatrix * vec4(a_position, 500);
}