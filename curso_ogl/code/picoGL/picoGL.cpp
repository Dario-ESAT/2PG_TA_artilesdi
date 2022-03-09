
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

#include "picoGL.h"
#include "picoGL_internal.h"

// Module vars
static SDL_Surface* g_surface      = 0;
static ARGBColor8888 g_clear_color = 0;
static Framebuffer g_fb            = {};

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
}

void picoglQuit ()
{
  SDL_Quit ();
}


void picoglBegin ()
{
  SDL_LockSurface (g_surface);
  g_fb.pixels = (ARGBColor8888*) g_surface->pixels;
}

void picoglClearColor (float r, float g, float b, float a)
{
  g_clear_color = (((int)(r * 255.0f))<<16) |
                  (((int)(g * 255.0f))<<8) |
                  ((int)(b * 255.0f)) |
                  (((int)(a * 255.0f))<<24);
}

void picoglClear ()
{
  SDL_FillRect (g_surface, NULL, g_clear_color);
}

void picoglEnd ()
{
  SDL_UnlockSurface (g_surface);
}

void picoglFlush ()
{
  SDL_Event event;

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


