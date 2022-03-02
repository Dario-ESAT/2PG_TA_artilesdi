#version 330
 
out vec4 fragColor;
uniform vec2 u_resolution = vec2(1280,720);

void main()
{ 
  float color = gl_FragCoord.x / u_resolution.x;

  fragColor = vec4(color,color,color,1);
}
