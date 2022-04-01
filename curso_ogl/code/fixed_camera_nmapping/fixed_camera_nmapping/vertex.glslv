
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;

out vec3 FragPos;
out vec2 TexCoord;
out vec3 Normal;
out vec3 Tangent;

uniform mat4 model;
uniform mat4 projection;

void main()
{
  mat3 model3x3 =  mat3(model);
  vec3 mT = model3x3 * aTangent;
  vec3 mN = model3x3 * aNormal;
  
  vec3 _mB = cross(mN, mT);
  mat3 TBN = mat3(mT,_mB,mN);

  vec3 N = Normal * 2.0 - 1.0;
  vec3 N = normalize(N);


  gl_Position = projection * model * vec4(aPos, 1.0f);
  FragPos = vec3(model * vec4(aPos,1.0));
  TexCoord = aTexCoord;
  Normal = TBN * N;
  Tangent = aTangent;
}


