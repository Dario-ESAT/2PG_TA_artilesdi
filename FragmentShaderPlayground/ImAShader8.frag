#version 330
 
out vec4 fragColor;
uniform vec2 u_resolution = vec2(1280,720);

void main()
{ 
  float blurr = 0.02f;
	fragColor = vec4(
    (gl_FragCoord.x - u_resolution.x / 2) * blurr,
    (gl_FragCoord.y - u_resolution.y / 2) * blurr,
    0, 1);  
}
