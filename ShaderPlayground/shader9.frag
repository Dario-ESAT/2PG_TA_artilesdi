#version 330
 
out vec4 fragColor;

in vec3 normal;

in float angle;

void main() {
  if(angle > 0.01){
    fragColor = vec4(1.0,0.0,0.0, 1.0);
  } else {
    fragColor = vec4(0.0,0.0,0.0, 1.0);
    
  }
  
	
 }
