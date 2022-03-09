
/* Copyright 2021 ESAT. All rights reserved.    #####  #####      #     #####
* Author: Luis Pons <lpons@esat.es>             #      #         # #      #
*                                               ###     ###     #   #     #
*                                               #          #   #     #    #
* Simple triangle rasterizer                    #####  #####  #       #   #

*/
#include <stdio.h>

#include "picoGL_internal.h"
#include "raster.h"


static inline Vec2 Vec2_add(Vec2 a, Vec2 b) 
{
  return (Vec2){a.x + b.x, a.y + b.y};
}
static inline Vec2 Vec2_sub(Vec2 a, Vec2 b) 
{
  return (Vec2){a.x - b.x, a.y - b.y};
}
static inline float Vec2_cross(Vec2 a, Vec2 b) 
{
  return a.x * b.y - a.y * b.x;
}

static inline float Min (float a, float b)
{
  return (a < b) ? a : b;
}

static inline float Max (float a, float b)
{
  return (a > b) ? a : b;
}


static inline int Clip (int v, int min, int max)
{
  if (v < min) v = min;
  if (v > max) v = max;
  return v;
}

// Linear blend of two colors
// Alpha
static inline int LerpFx8 (int a, int b, int alpha)
{
    return b + ((alpha * (a - b)) >> 8);
}

static inline ARGBColor8888 BlendColor (ARGBColor8888 p0, ARGBColor8888 p1, unsigned int alpha)
{
  // Scale 0-255 range to 0-256 for fx8 operation
  unsigned int nalpha = (alpha * 257) >> 8;
  unsigned int a =  LerpFx8 (p0 >> 24,          p1 >> 24,          nalpha);
  unsigned int r =  LerpFx8 ((p0 >> 16) & 0xff, (p1 >> 24) & 0xff, nalpha);
  unsigned int g =  LerpFx8 ((p0 >> 8) & 0xff,  (p1 >> 8) & 0xff,  nalpha);
  unsigned int b =  LerpFx8 (p0 & 0xff,         p1 & 0xff,         nalpha);
  return (a << 24) | (r << 16) | (g << 8) | b;
}

// Triangle rasterizer, with culling & alpha blending
// (highly inefficient)

void DrawTriangle (Framebuffer* fb, PicoContext* ctx, Vec2* vtx)
{
  // Bounding box of the triangle
  int x1 = (int)Min(Min(vtx[0].x, vtx[1].x), vtx[2].x);
  int y1 = (int)Min(Min(vtx[0].y, vtx[1].y), vtx[2].y);
  int x2 = (int)Max(Max(vtx[0].x, vtx[1].x), vtx[2].x);
  int y2 = (int)Max(Max(vtx[0].y, vtx[1].y), vtx[2].y);

  // Clip to screen coordinates
  x1 = Clip(x1, 0, fb->w - 1);
  x2 = Clip(x2, 0, fb->w - 1);
  y1 = Clip(y1, 0, fb->h - 1);
  y2 = Clip(y2, 0, fb->h - 1);

  int x, y;
  for (y=y1; y<=y2; y++) {
    for (x=x1; x<=x2; x++) {   
      int negs = 0;
      Vec2 ss = (Vec2){(float)x, (float)y};
      Vec2 vp0 = Vec2_sub (ss, vtx[0]);
      Vec2 v0 = Vec2_sub (vtx[1], vtx[0]);
      Vec2 vp1 = Vec2_sub (ss, vtx[1]);     
      Vec2 v1 = Vec2_sub (vtx[2], vtx[1]);
      Vec2 vp2 = Vec2_sub (ss, vtx[2]);
      Vec2 v2 = Vec2_sub (vtx[0], vtx[2]);
      if (Vec2_cross(vp0, v0) < 0.0f)
        negs++;
      if (Vec2_cross(vp1, v1) < 0.0f)
        negs++;
      if (Vec2_cross(vp2, v2) < 0.0f)
        negs++;
      
      bool draw = false;  
      if (((negs == 3) || (negs == 0)) && (ctx->culling == kNoCulling))
        draw = true;
      else if ((negs == 3) && (ctx->culling == kCullBack))
        draw = true;
      else if ((negs == 0) && (ctx->culling == kCullFront))
        draw = true;
      
      if (draw) {
        unsigned int color = ctx->color;
        unsigned int back_color = fb->pixels[x + y* fb->w];
        
        if (ctx->blend == kBlendSrcAlpha)
          color = BlendColor(color, back_color, color >> 24);
        else if (ctx->blend == kBlendDstAlpha)
          color = BlendColor(color, back_color, back_color >> 24);
          
        fb->pixels[x + y* fb->w] = color;
      }
    }
  }
      
}








