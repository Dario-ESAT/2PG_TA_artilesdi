#version 330
 
out vec4 fragColor;
uniform vec2 u_resolution = vec2(1280,720);

void main() { 
  vec3 circleColor = vec3(gl_FragCoord.y / u_resolution.y, 0.35, gl_FragCoord.x / u_resolution.x);
  float thickness = 0.5;
  // Control with mouse
  // thickness = iMouse.x / u_resolution.x;
  float fade = 0.005;

  // -1 -> 1 local space, adjusted for aspect ratio
  vec2 uv = gl_FragCoord.xy / u_resolution.xy * 3.0 - 1.5;
  float aspect = u_resolution.x / u_resolution.y;
  uv.x *= aspect;
  
  // Calculate distance and fill circle with white
  float distance = 1.0 - length(uv);
  vec3 color = vec3(smoothstep(0.0, fade, distance));
  color *= vec3(smoothstep(thickness + fade, thickness, distance));

  // Set output color
  fragColor = vec4(color, 1.0);
  fragColor.rgb *= circleColor;
}
