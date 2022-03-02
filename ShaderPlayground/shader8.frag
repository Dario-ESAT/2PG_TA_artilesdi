#version 330
 
out vec4 fragColor;

in vec3 normal;
in vec3 modelpos;

void main() { 
  float biggerthan = step(modelpos.y,19.9);
	
  fragColor = vec4(biggerthan,biggerthan, biggerthan, 1.0);
  
	
 }
