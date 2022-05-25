#version 330

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D postproc_image;

void main() { 
  vec2 tex_sz = textureSize(postproc_image,0);
  vec2 texel_size = 1.0f / tex_sz;

  vec4 sample_left = texture(postproc_image, vec2(TexCoord.x - texel_size.x, TexCoord.y));

  vec4 sample = texture(postproc_image, TexCoord);
  FragColor = sample * 0.5f - sample_left * 0.5f;  

}

