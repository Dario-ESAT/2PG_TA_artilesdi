#version 330

uniform float u_time;
uniform vec3 u_view_pos;
uniform mat4 u_m_matrix;
uniform mat4 u_vp_matrix;


layout (location=0) in vec3 a_position;
layout (location=1) in vec3 a_normal;

out vec3 normal;
out vec3 w_pos;

void main()
{
  normal = normalize(mat3(u_m_matrix) * a_normal);
  
  w_pos = (u_m_matrix * vec4(a_position, 1.0)).xyz;
  // normal = normalize(a_normal);
	gl_Position = u_vp_matrix * u_m_matrix * vec4(a_position, 1.0);
}
