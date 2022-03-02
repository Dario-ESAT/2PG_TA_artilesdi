#version 330
 
out vec4 fragColor;
uniform vec2 u_resolution = vec2(1280,720);

void main()
{ 
	fragColor = vec4(
  step(0.3,sin(gl_FragCoord.x * 0.06) / cos(gl_FragCoord.y * 0.06)),
  step(0.3,sin(gl_FragCoord.x * 0.06) / cos(gl_FragCoord.y * 0.06)),
  step(0.3,sin(gl_FragCoord.x * 0.06) / cos(gl_FragCoord.y * 0.06)),
  1);
}
