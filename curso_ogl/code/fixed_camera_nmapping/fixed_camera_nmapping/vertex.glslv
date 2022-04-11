
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;

out vec3 FragPos;
out vec2 TexCoord;
out mat3 TBN;

uniform mat4 model;
uniform mat4 projection;

void main() {
  gl_Position = projection * model *
  vec4(aPos, 1.0f);

  mat3 model3x3 =  mat3(model);
  vec3 mT = model3x3 * aTangent;
  vec3 mN = model3x3 * aNormal;
  
  vec3 _mB = cross(mN, mT);
  TBN = mat3(mT,_mB,mN);

  // Normal = aNormal;
  FragPos = vec3(model * vec4(aPos,1.0));
  TexCoord = aTexCoord;
}


