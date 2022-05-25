
#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoord;
in vec3 mT;
in vec3 mN;

uniform vec3 PointLightPos;
// Texture samplers
uniform sampler2D tex_image;
uniform sampler2D tex_normals;

 
void main()
{
  vec3 _mB = cross(mN, mT);
  mat3 TBN = mat3(mT,_mB,mN); 
  vec3 N = vec3(texture(tex_normals, TexCoord));
  N = N * 2.0 - 1.0;
  // El componente Z no esta en la textura y lo tenemos que reconstruir
  N.z = sqrt(1.0 - (N.x* N.x + N.y * N.y));
  N = TBN * N;          // Lo llevamos de espacio tangencial a mundo

  // Iluminacion difusa simple
  vec3 L = normalize(PointLightPos - FragPos);
  float i = max(dot(L, N),0.0f); 

  FragColor = i * texture(tex_image, TexCoord);
}
