#version 330
 
out vec4 fragColor;
uniform vec2 u_resolution = vec2(1280,720);

void main()
{ 
	fragColor = vec4(sin(gl_FragCoord.x * 0.05), sin(gl_FragCoord.y * 0.05), 0, 1);  
}
