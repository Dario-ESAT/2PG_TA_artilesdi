#version 330

uniform float u_time;

out vec4 fragColor;
 
in vec3 normal;

vec3 light_dirR = vec3(1,0,0);
vec3 light_dirL = vec3(-1,0,0);
vec3 light_col = vec3(1,1,1);

void main() {
  vec3 n_normal = normalize(normal);
  float dotpR = max(dot(n_normal,light_dirR),0);
  float dotpL = max(dot(n_normal,light_dirL),0);

	fragColor = vec4(light_col * dotpR + light_col * dotpL, 1.0);
	
 }
