
#version 330 core
out vec4 FragColor;
uniform vec4 RawColor;

in vec3 a_norm;

void main()
{
  //FragColor = RawColor; //vec4(1.0f, 0.0f, 0.0f, 0.5f);
  FragColor = vec4(a_norm, 0.5f);
}

