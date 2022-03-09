
#include <math.h>
// Temporal includes 
#include "picoGL_internal.h"
#include "raster.h"
// Pico GL include
#include "picoGL.h"


#define PI 3.141592f

static void DrawRing (PicoContext* ctx, float r0, float r1, float cx, float cy, bool reverse, float aoff)
{
  int i;
  const int kSegments = 20;
  for (i=0; i<kSegments; i++) {
    float a0 = (2.0f * PI * (float)i) / (float)kSegments;
    float a1 = (2.0f * PI * (float)(i + 1)) / (float)kSegments;
    a0 += aoff;
    a1 += aoff;
    Vec2 quad [4] = {
        {cx + cosf(a0) * r1, cy + sinf(a0) * r1},
        {cx + cosf(a1) * r1, cy + sinf(a1) * r1},
        {cx + cosf(a1) * r0, cy + sinf(a1) * r0},
        {cx + cosf(a0) * r0, cy + sinf(a0) * r0},
    };

    if (! reverse) {
      Vec2 tri0[3] = {quad [0], quad [1], quad [2]};
      DrawTriangle ((Framebuffer*)picoglGetFramebuffer(), ctx, tri0);
      Vec2 tri1[3] = {quad [2], quad [3], quad [0]};
      DrawTriangle ((Framebuffer*)picoglGetFramebuffer(), ctx, tri1);
    }
    else {
      Vec2 tri0[3] = {quad [0], quad [2], quad [1]};
      DrawTriangle ((Framebuffer*)picoglGetFramebuffer(), ctx, tri0);
      Vec2 tri1[3] = {quad [2], quad [0], quad [3]};
      DrawTriangle ((Framebuffer*)picoglGetFramebuffer(), ctx, tri1);
    }
  }
}

static float t = 0.0f;

int main (int argc, char** argv)
{
  picoglInit   (800, 600);

  picoglClearColor (0.0f, 0.0f, 1.0f, 1.0f);
  
  while (1) {
    picoglClear  ();
    picoglBegin  ();
    
    PicoContext ctx0 = {
      .culling = kCullBack,
      .blend = kNoBlend,
      .color = 0xff00ff00,
    };

    DrawRing (&ctx0, 150.0f, 240.0f, 400.0f, 300.0f, false, t);

    PicoContext ctx1 = {
      .culling = kCullFront,
      .blend = kBlendSrcAlpha,
      .color = 0x80ff0000,
    };

    DrawRing (&ctx1, 220.0f, 280.0f, 400.0f, 300.0f, true, -t);
    
    picoglEnd    ();
    picoglFlush  ();

    //t += 0.01f;
  }
  
  picoglQuit   ();
  
  return 1;
}
