
#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;
in vec3 Tangent;

uniform vec4 RawColor;
uniform vec3 PointLightPos;

uniform sampler2D text_diffuse;
uniform sampler2D text_normals;

 
void main()
{
  // Iluminacion difusa simple
  vec3 L = normalize(PointLightPos - FragPos);
  float i = max(dot(L, Normal),0.0f);

  FragColor = texture(text_normals, TexCoord) * i;
  // FragColor = vec4(Tangent,1);

}
