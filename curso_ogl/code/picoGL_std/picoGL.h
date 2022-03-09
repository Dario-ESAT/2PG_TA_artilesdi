
#ifndef _PICOGL_H_
#define _PICOGL_H_


#define PGL_CULL_FACE  (0x0B44)
#define PGL_BLEND      (0x0BE2)

#define PGL_FRONT      (0x0404)
#define PGL_BACK       (0x0405)

#define PGL_SRC_ALPHA             (0x0302)
#define PGL_ONE_MINUS_SRC_ALPHA   (0x0303)
#define PGL_DST_ALPHA             (0x0304)
#define PGL_ONE_MINUS_DST_ALPHA   (0x0305)

void picoglInit        (int w, int h);
void picoglQuit        ();
void picoglClearColor  (float r, float g, float b, float a);
void picoglClear       ();
void picoglSwapBuffers ();

void picoglEnable       (PGLenum cap);
void picoglDisable      (PGLenum cap);
void picoglCullFace     (PGLenum mode);
void picoglBlendFunc    (PGLenum sfactor, PGLenum dfactor);
void picoglDrawTris     (float* vtx, unsigned int *indices, int count);
// Triangle color
void glColor4f          (float r, float g, float b, float a);


#endif 


