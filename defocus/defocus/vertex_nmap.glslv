
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 N;
layout (location = 3) in vec3 T;

out vec3 FragPos;
out vec2 TexCoord;
out vec3 mT;
out vec3 mN;

uniform mat4 model;
uniform mat4 projection;

void main()
{
  mat3 model3x3 = mat3(model); 
  TexCoord = aTexCoord;
  gl_Position = projection * model * vec4(aPos, 1.0f);
  FragPos = vec3(model * vec4(aPos,1.0));
  // Llevamos los vectores directores de la matriz (incompleta) que define espacio tangencial a mundo
  mT = model3x3 * T;
  mN = model3x3 * N;
}


