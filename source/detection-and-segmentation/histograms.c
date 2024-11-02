#include <SDL2/SDL.h>

#include "histograms.h"

void get_col_histogram(SDL_Surface *surface, int **histogram) {
  for (int i = 0; i < surface->w; i++) {
    for (int j = 0; j < surface->h; j++) {
      Uint32 pixel = ((Uint32 *)surface->pixels)[j * surface->w + i];
      Uint8 r;
      Uint8 g;
      Uint8 b;
      SDL_GetRGB(pixel, surface->format, &r, &g, &b);
      if (r != 0)
        (*histogram)[i] += 1;
    }
  }
}

void get_row_histogram(SDL_Surface *surface, int **histogram) {
  for (int j = 0; j < surface->h; j++) {
    for (int i = 0; i < surface->w; i++) {
      Uint32 pixel = ((Uint32 *)surface->pixels)[j * surface->w + i];
      Uint8 r;
      Uint8 g;
      Uint8 b;
      SDL_GetRGB(pixel, surface->format, &r, &g, &b);
      if (r != 0)
        (*histogram)[j] += 1;
    }
  }
}
