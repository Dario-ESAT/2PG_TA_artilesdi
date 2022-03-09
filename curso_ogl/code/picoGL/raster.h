
#ifndef _RASTER_H_
#define _RASTER_H_


enum CullingMode
{
  kNoCulling,
  kCullFront,
  kCullBack
};

enum BlendingType
{
  kNoBlend,
  kBlendSrcAlpha,
  kBlendDstAlpha
};

typedef struct
{
  CullingMode   culling;
  BlendingType  blend;
  ARGBColor8888 color;
} PicoContext;

typedef struct
{
  float x, y;
} Vec2;

void DrawTriangle (Framebuffer* fb, PicoContext* ctx, Vec2* vtx);

#endif




