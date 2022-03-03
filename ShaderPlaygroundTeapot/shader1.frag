#version 330

uniform float u_time;

out vec4 fragColor;
 
in vec3 normal;

vec3 light_dir = vec3(0,1,0);
vec3 light_col = vec3(0,0,1);

void main() {
  vec3 n_normal = normalize(normal);
  float dotp = dot(n_normal,light_dir);
	fragColor = vec4(light_col * dotp, 1.0);
	
 }
