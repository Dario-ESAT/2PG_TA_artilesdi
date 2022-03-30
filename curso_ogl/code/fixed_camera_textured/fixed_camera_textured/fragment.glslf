
#version 330 core
out vec4 FragColor;

in vec3 Normal; 
in vec3 FragPos;
in vec2 TexCoord;

uniform vec4 RawColor;
uniform vec3 PointLightPos;
// Texture sampler
uniform sampler2D tex_diffuse;
 
void main()
{
  vec3 L = normalize(PointLightPos - FragPos);
  vec3 N = normalize(Normal);
  float i = max(dot(L, N),0.0f);

  FragColor = i * texture(tex_diffuse, TexCoord);
}
