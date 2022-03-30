#version 330
 
uniform vec3 u_color;
uniform vec3 u_view_pos;
out vec4 fragColor;

in vec3 normal;
in vec3 w_pos;
 

vec3 light_dir = normalize(vec3(0,-1,0));
vec3 light_col = vec3(0.5,0.5,0.7);
vec3 specular_color = vec3(1,1,1);
vec3 ambient = vec3(0.003,0.003,0.03);

float material_shininess = 132.0;
void main() { 

  vec3 n_normal = normalize(normal);
  
  float difusse = max(dot(n_normal,-light_dir),0.0);

  vec3 bounce = reflect(light_dir,n_normal);
  vec3 camera_dir = normalize(w_pos - u_view_pos);
  float specular = pow(max(dot(-bounce,camera_dir),0.0),material_shininess);

  vec3 light = (((u_color * difusse) + (specular * specular_color)) * light_col) + ambient;


	fragColor = vec4(light, 1.0);	
}
