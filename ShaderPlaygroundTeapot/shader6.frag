#version 330

uniform float u_time;

out vec4 fragColor;
 
in vec3 normal;
in float light_y;
in vec3 local_pos;
float constant = 1;
float linear = 0;
float quadratic = 0;

vec3 light_col = vec3(1,1,1);

void main() {
  vec3 lightPos = vec3(-5.0, light_y, 0.0);
  vec3 n_normal = normalize(normal);
  vec3 lightDir = (gl_FragCoord.xyz - lightPos);
  float Distance = length(lightDir);
  lightDir = normalize(lightDir);

  float Attenuation = constant +
                      linear * Distance +
                      quadratic * Distance * Distance;

  vec3 final_color = light_col * dot(n_normal,-lightDir) / Attenuation;

  fragColor = vec4(final_color, 1.0);
	
 }
