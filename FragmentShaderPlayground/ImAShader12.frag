#version 330
 
out vec4 fragColor;
uniform vec2 u_resolution = vec2(1280,720);

void main()
{
  float x = step(fract(gl_FragCoord.x * 0.015), 0.5);
  float y = step(fract(gl_FragCoord.y * 0.015), 0.5);

  float color = x * y;
  fragColor = vec4( color, color, color, 1);
}
