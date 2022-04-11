
#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoord;
in mat3 TBN;

uniform vec4 RawColor;
uniform vec3 PointLightPos;

uniform sampler2D text_diffuse;
uniform sampler2D text_normals;

 
void main() {

  vec3 normal = texture(text_normals, TexCoord).xyz;
  normal.z = sqr(1 - normal.x * normal.x + normal.y * normal.y);
  vec3 N = normal * 2.0 - 1.0;
  N = normalize(N);
  N = TBN * N;

  // Iluminacion difusa simple
  vec3 LD = normalize(PointLightPos - FragPos);
  float i = max(dot(LD, N),0.0f);
  FragColor = texture(text_diffuse, TexCoord) * i * RawColor;
  // FragColor = vec4(N,1.0);
}
