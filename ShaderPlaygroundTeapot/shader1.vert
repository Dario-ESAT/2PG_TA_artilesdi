#version 330

uniform float u_time;
uniform mat4 u_m_matrix;
uniform mat4 u_vp_matrix;

layout (location=0) in vec3 a_position;
layout (location=1) in vec3 a_normal;

out vec3 normal;

void main() {
    normal = mat3(u_m_matrix) * a_normal;
    gl_Position = u_vp_matrix * u_m_matrix * vec4(a_position * 3.5, 1.0);
}
