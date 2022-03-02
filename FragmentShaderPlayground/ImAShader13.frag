#version 330
 
out vec4 fragColor;
uniform vec2 u_resolution = vec2(1280,720);

float drawLine (vec2 p1, vec2 p2, vec2 uv, float a) {
    
    float r = 0.0;
    float one_px = 1.0 / u_resolution.x; //not really one px
    
    // get dist between points
    float d = distance(p1, p2);
    
    // get dist between current pixel and p1
    float duv = distance(p1, uv);

    //if point is on line, according to dist, it should match current uv
    r = 1.-floor(1.0 - (a * one_px) + distance(mix(p1, p2, clamp(duv / d, 0.0, 1.0)),  uv));

    return r;
}


void main() { 
  vec2 uv = gl_FragCoord.xy / u_resolution.xy;

  vec2 p1 = vec2(0.2,1);
  vec2 p2 = vec2(0.8,0);
  vec2 p3 = vec2(0.8,1);
  vec2 p4 = vec2(0.2,0);
  
  
  
  float lines = drawLine(p1, p2, uv, 1.0)
            + drawLine(p3, p4, uv, 1.0);


	fragColor = vec4(lines,lines,0,1);
}
