#version 330

uniform vec3 u_view_pos;
uniform mat4 u_m_matrix;
uniform mat4 u_vp_matrix;

layout (location=0) in vec3 a_position;
layout (location=1) in vec3 a_normal;

out vec3 normal;

void main()
{
    normal = normalize((u_m_matrix) * vec4(a_normal,0)).xyz;
   gl_Position = (u_vp_matrix*u_m_matrix) * vec4(a_position, 1);
}