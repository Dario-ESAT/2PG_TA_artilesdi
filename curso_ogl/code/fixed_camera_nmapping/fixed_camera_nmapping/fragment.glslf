
#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoord;

uniform vec4 RawColor;
uniform vec3 PointLightPos;

uniform sampler2D tex_image;

 
void main()
{
  // Iluminacion difusa simple
  //vec3 L = normalize(PointLightPos - FragPos);
  //float i = max(dot(L, N),0.0f); 

  FragColor = texture(tex_image, TexCoord);
}
