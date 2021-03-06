
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 Normal; 
out vec3 FragPos;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 projection;

void main()
{
  TexCoord = aTexCoord; // Normalmente no requiere ninguna transformacion
  gl_Position = projection * model * vec4(aPos, 1.0f);
  Normal = mat3(model) * aNormal;
  FragPos = vec3(model * vec4(aPos,1.0));
}


