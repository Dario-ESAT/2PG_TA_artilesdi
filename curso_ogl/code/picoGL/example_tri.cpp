
// Temporal includes 
#include "picoGL_internal.h"
#include "raster.h"
// Pico GL include
#include "picoGL.h"



int main (int argc, char** argv)
{
  picoglInit   (800, 600);

  picoglClearColor (0.3f, 0.5f, 1.0f, 1.0f);
  
  while (1) {

    picoglBegin  ();

    picoglClear  ();
        
    PicoContext ctx = {
      .culling = kCullFront,
      .blend = kNoBlend,
      .color = 0xff0000ff,
    };
    
    Vec2 tri [3] = {
      {100.0f, 100.0f},
      {200.0f, 300.0f},
      {500.0f, 500.0f},
    };
    
    DrawTriangle ((Framebuffer*)picoglGetFramebuffer(), &ctx, tri);

    picoglEnd    ();
    picoglFlush  ();
  }
  
  picoglQuit   ();
  
  return 1;
}
