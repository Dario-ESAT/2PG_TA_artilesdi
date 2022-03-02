
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;

uniform mat4 model;
uniform mat4 projection;

out vec3 a_norm;
void main()
{
	a_norm = aNorm;
	gl_Position = projection * model * vec4(aPos, 1.0f);
}


