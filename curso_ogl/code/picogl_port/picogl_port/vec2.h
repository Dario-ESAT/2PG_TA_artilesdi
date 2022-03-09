
#ifndef _VEC2_H_
#define _VEC2_H_

typedef struct
{
  float x, y;
} Vec2;

static inline Vec2 Vec2_add(Vec2 a, Vec2 b) 
{
  return {a.x + b.x, a.y + b.y};
}
static inline Vec2 Vec2_sub(Vec2 a, Vec2 b) 
{
  return {a.x - b.x, a.y - b.y};
}
static inline float Vec2_cross(Vec2 a, Vec2 b) 
{
  return a.x * b.y - a.y * b.x;
}

#endif


