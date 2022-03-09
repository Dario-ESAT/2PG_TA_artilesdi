
/* Copyright 2021 ESAT. All rights reserved.    #####  #####      #     #####
* Author: Luis Pons <lpons@esat.es>             #      #         # #      #
*                                               ###     ###     #   #     #
*                                               #          #   #     #    #
* Toy "OGL like" graphic lib                    #####  #####  #       #   #
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <SDL/SDL.h>
#include <assert.h>

#include "picoGL_types.h"
#include "picoGL.h"
#include "vec2.h"
#include "raster.h"

// Module vars
static SDL_Surface* g_surface      = 0;
static ARGBColor8888 g_clear_color = 0;
static Framebuffer g_fb            = {};

// Render Context    
static PicoContext gRenderCtx = {
  .culling = kCullBack,
  .blend = kNoBlend,
  .color = 0x00000000,
};

static CullingMode   gCulling = kCullBack;
static BlendingType  gBlend   = kBlendSrcAlpha;

void* picoglGetFramebuffer ()
{
  return (void*) &g_fb;
}

void picoglInit (int w, int h)
{
  // Init SDL and screen
  if ( SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0 ) {
    fprintf(stderr, "Can't Initialise SDL: %s\n", SDL_GetError());
    exit(1);
  }

  if (0 == SDL_SetVideoMode( w, h, 32,  SDL_HWSURFACE | SDL_DOUBLEBUF)) {
    printf("Couldn't set %dx%dx32 video mode: %s\n", w, h, SDL_GetError());
    exit(1);
  }
  
  g_surface = SDL_GetVideoSurface();
  
  SDL_LockSurface (g_surface);
  g_fb.w = g_surface->w;
  g_fb.h = g_surface->h;
  SDL_UnlockSurface (g_surface);
  
  SDL_LockSurface (g_surface);
  g_fb.pixels = (ARGBColor8888*) g_surface->pixels;
}

void picoglQuit ()
{
  SDL_Quit ();
}



static inline unsigned int colorF2ARGB (float r, float g, float b, float a)
{
  return  (((int)(r * 255.0f))<<16) |
           (((int)(g * 255.0f))<<8) |
           ((int)(b * 255.0f)) |
           (((int)(a * 255.0f))<<24);
}

void picoglClearColor (float r, float g, float b, float a)
{
  g_clear_color = colorF2ARGB (r,g,b,a);
}

void picoglClear ()
{
  SDL_FillRect (g_surface, NULL, g_clear_color);
}

void picoglSwapBuffers ()
{
  SDL_Event event;

  SDL_UnlockSurface (g_surface);

  SDL_Flip (g_surface);

  // Check input events
  while ( SDL_PollEvent(&event)) {
      switch (event.type)  {
          //case SDL_MOUSEMOTION:
          //    mouse_x = event.motion.x;
          //    mouse_y = event.motion.y;
          //    break;
          //case SDL_MOUSEBUTTONDOWN:
          //    break;
          case SDL_QUIT:
              exit(0);
              break;
      }
  }
}


void picoglEnable  (PGLenum cap)
{
  assert ((cap == PGL_CULL_FACE) || (cap == PGL_BLEND));
  if (cap == PGL_CULL_FACE) 
    gRenderCtx.culling = gCulling;
  if (cap == PGL_BLEND) 
    gRenderCtx.blend = gBlend;
}

void picoglDisable      (PGLenum cap)
{
  assert ((cap == PGL_CULL_FACE) || (cap == PGL_BLEND));
  if (cap == PGL_CULL_FACE) 
    gRenderCtx.culling = kNoCulling;
  if (cap == PGL_BLEND) 
    gRenderCtx.blend = kNoBlend;

}

void picoglCullFace  (PGLenum mode)
{
  assert((mode == PGL_FRONT) || (mode == PGL_BACK)); 
  if (mode == PGL_FRONT)
    gCulling = kCullFront;
  if (mode == PGL_BACK)
    gCulling = kCullBack;
  if (gRenderCtx.culling != kNoCulling) 
    gRenderCtx.culling = gCulling; // blend was enable, update mode
   
}

void picoglBlendFunc    (PGLenum sfactor, PGLenum dfactor)
{
  assert((sfactor == PGL_SRC_ALPHA) || (sfactor == PGL_DST_ALPHA));
  if (sfactor == PGL_SRC_ALPHA) {
    assert(dfactor == PGL_ONE_MINUS_SRC_ALPHA);
    gBlend = kBlendSrcAlpha;
  } 
  if (sfactor == PGL_DST_ALPHA) {
    assert(dfactor == PGL_ONE_MINUS_DST_ALPHA);
    gBlend = kBlendDstAlpha;
  } 
  if (gRenderCtx.blend != kNoBlend) 
    gRenderCtx.blend = gBlend; // blend was enable, update mode

}

void glColor4f(float r, float g, float b, float a)   // Triangle color
{
  gRenderCtx.color = colorF2ARGB (r,g,b,a);
}

void picoglDrawTris (float* vtx, unsigned int *indices, int count)
{
  Vec2* v = (Vec2*) vtx; 
  int i;
  for(i=0; i<count ; i++){
    int i0 = *indices++;
    int i1 = *indices++;
    int i2 = *indices++;
    Vec2 tri[3] = {v[i0], v[i1], v[i2]};
    DrawTriangle (&g_fb, &gRenderCtx, tri);
  }

}




