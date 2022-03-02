#version 330
 
out vec4 fragColor;
uniform vec2 u_resolution = vec2(1280,720);

void main()
{ 
  float blurr = 0.02f;

	fragColor = vec4(
  sin(gl_FragCoord.x * 0.02 + gl_FragCoord.y * 0.02),
  sin(gl_FragCoord.x * 0.02 + gl_FragCoord.y * 0.02),
  0,
  1);
}
