
#include <math.h>
// Pico GL include
#include "vec2.h"
#include "picoGL_types.h"
#include "picoGL.h"

#define PI 3.141592f

#define N_SEGMENTS  (20)

static Vec2 gRings [N_SEGMENTS * 2];

static unsigned int g_MeshIndices [N_SEGMENTS * 3 * 2]; //2 tris = 1 quad

static void DrawRing (float inner, float outer, float cx, float cy, bool reverse, float aoff)
{
  int i;
  int outer_offs = N_SEGMENTS;
  for (i=0; i<N_SEGMENTS; i++) {
    float a = (2.0f * PI * (float)i) / (float)N_SEGMENTS;
    float anim = a + aoff;
    gRings [i]            = (Vec2){cx + cosf(anim) * inner, cy + sinf(anim) * inner};
    gRings [i+outer_offs] = (Vec2){cx + cosf(anim) * outer, cy + sinf(anim) * outer};
  }

 unsigned int * idx = g_MeshIndices;
 for (i=0; i<N_SEGMENTS; i++) {
    int next = (i + 1) < N_SEGMENTS ? i+1 : 0;
    if (!reverse) {
      idx[0] = outer_offs + i; 
      idx[1] = outer_offs + next; 
      idx[2] = i; 
      idx[3] = i; 
      idx[4] = outer_offs + next; 
      idx[5] = next;
    }
    else {
      idx[0] = outer_offs + i; 
      idx[2] = outer_offs + next; 
      idx[1] = i; 
      idx[3] = i; 
      idx[5] = outer_offs + next; 
      idx[4] = next;
    }
    idx += 6;
  }
  
  picoglDrawTris ((float*)gRings, g_MeshIndices, N_SEGMENTS * 2);
}

static float t = 0.0f;


int main (int argc, char** argv)
{
  picoglInit   (800, 600);

  picoglEnable       (PGL_CULL_FACE);
  picoglDisable      (PGL_BLEND);

  picoglBlendFunc    (PGL_SRC_ALPHA, PGL_ONE_MINUS_SRC_ALPHA);

  while (1) {
    picoglClearColor (0.2f, 0.3f, 0.3f, 1.0f);
    picoglClear  ();

    glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
    picoglCullFace     (PGL_BACK);

    DrawRing (150.0f, 240.0f, 400.0f, 300.0f, false, t);
    
    glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
    picoglEnable       (PGL_BLEND);
    picoglCullFace     (PGL_FRONT);

    DrawRing (220.0f, 280.0f, 400.0f, 300.0f, true, -t);

    picoglDisable      (PGL_BLEND);

    picoglSwapBuffers ();

    t += 0.01f;
  }
  
  picoglQuit   ();
  
  return 1;
}
