#version 330

uniform float u_time;
uniform mat4 u_m_matrix;
uniform mat4 u_vp_matrix;

layout (location=0) in vec3 a_position;
layout (location=1) in vec3 a_normal;

vec3 light_dir = vec3(0,0,1);
vec3 light_col = vec3(1,1,1);
out vec3 color;
vec3 normal;
void main() {

  normal = mat3(u_m_matrix) * a_normal;
  vec3 n_normal = normalize(normal);

  float dotp = dot(n_normal,light_dir);
  if(dotp < 0.9) {
    dotp = 0.0;
  }
  color = light_col * dotp;

  gl_Position = u_vp_matrix * u_m_matrix * vec4(a_position * 3.5, 1.0);
}
