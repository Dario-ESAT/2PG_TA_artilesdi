#version 330
 
uniform sampler2D u_colorTexture;

out vec4 fragColor;
in vec3 pos;
in vec2 uv;
in vec4 customData;

void main() { 
	fragColor = texture(u_colorTexture, vec2(uv.x * 6, uv.y * 6 + customData.x * 0.0003));
}

//Ej 1 -----
//void main() { 
//	fragColor = texture(u_colorTexture, uv);		
// }
//-----


//Ej 4 -----
//void main() { 
//	fragColor = texture(u_colorTexture, uv * 6);		
// }
//-----