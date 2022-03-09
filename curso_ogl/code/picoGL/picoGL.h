
#ifndef _PICOGL_H_
#define _PICOGL_H_

void picoglInit   (int w, int h);
void picoglQuit   ();
void picoglClearColor (float r, float g, float b, float a);
void picoglClear  ();
void picoglBegin  ();
void picoglEnd    ();
void picoglFlush  ();


// Hackish bridge; only valid between picoglBegin/picoglEnd calls
void* picoglGetFramebuffer ();

#endif 


