#version 330

uniform float u_time;

out vec4 fragColor;
 
in vec3 normal;
in float light_y;
in vec3 local_pos;

float constant = 0;
float linear = 0.2;
float quadratic = 0;
float cut = cos(100);
vec3 light_col = vec3(1,1,1);
vec3 light_dir = vec3(1,0,0);


void main() {
  vec3 lightPos = vec3(1.0, 0, 0.0);
  vec3 n_normal = normalize(normal);

  vec3 light_to_pixel = normalize(local_pos - lightPos);
  float spot_factor = dot(light_to_pixel,light_dir);



  vec3 final_color;

  if(spot_factor > cut){
    vec3 lightDir = (gl_FragCoord.xyz - lightPos);
    float Distance = length(lightDir);
    lightDir = normalize(lightDir);

    float Attenuation = constant +
                        linear * Distance +
                        quadratic * Distance * Distance;

    final_color = light_col * dot(n_normal,-lightDir) / Attenuation;

    float intensity = 1.0 - (1.0 - spot_factor) / (1.0 - cut);
    final_color = final_color * intensity;
  } else {
    final_color = vec3(0,0,0);
  }

  fragColor = vec4(final_color, 1.0);
	
 }
