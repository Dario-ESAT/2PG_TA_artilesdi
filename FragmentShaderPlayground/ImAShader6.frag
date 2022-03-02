#version 330
 
out vec4 fragColor;
uniform vec2 u_resolution = vec2(1280,720);

void main()
{ 
	if(gl_FragCoord.x > u_resolution.x / 2)
    fragColor = vec4(1,0,0,1);
  else
    fragColor = vec4(0,0,0,1);
}
