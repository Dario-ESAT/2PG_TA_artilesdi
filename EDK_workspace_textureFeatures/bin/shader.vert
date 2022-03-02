#version 330

uniform float u_time;
uniform mat4 u_m_matrix;
uniform mat4 u_vp_matrix;
uniform vec4 u_customData;

layout (location=0) in vec3 a_position;
layout (location=1) in vec2 a_uv;

out vec3 pos;
out vec2 uv;
out vec4 customData;
void main()
{
	gl_Position = u_vp_matrix * vec4(a_position, 1.0);
	
	pos = a_position;
	uv = a_uv;
	customData = u_customData;
}
