#ifndef DECOLORER_H
#define DECOLORER_H

#include <SDL2/SDL.h>

Uint32 change_pixel(SDL_PixelFormat *format,
	Uint8 nr, Uint8 ng, Uint8 nb);
Uint32 to_grayscale(SDL_PixelFormat* format, Uint32 pixel);
void image_to_grayscale(SDL_Surface* surface);
void get_histogram(SDL_Surface* g_image, int[256] histogram);

#endif
