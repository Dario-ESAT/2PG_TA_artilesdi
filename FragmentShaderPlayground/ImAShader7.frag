#version 330
 
out vec4 fragColor;
uniform vec2 u_resolution = vec2(1280,720);

void main()
{ 
	fragColor = vec4(gl_FragCoord.x - u_resolution.x / 2, 0, 0, 1);  
}
