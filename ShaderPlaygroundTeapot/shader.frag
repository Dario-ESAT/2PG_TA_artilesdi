#version 330

uniform float u_time;

out vec4 fragColor;
 
in vec3 normal;

void main() { 
  vec3 n_normal = normalize(normal);
  float dotp = dot(n_normal,vec3(-1,0,0));
	fragColor = vec4(dotp,dotp,dotp, 1.0);
	
 }
