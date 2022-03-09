
#version 330 core
out vec4 FragColor;
uniform vec4 RawColor;
void main()
{
  FragColor = RawColor; //vec4(1.0f, 0.0f, 0.0f, 0.5f);
}

