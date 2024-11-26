#ifndef PIXEL_UTILS_H
#define PIXEL_UTILS_H

#include <SDL2/SDL.h>

void set_gpixel_from_coord(SDL_Surface* surface, int x, int y, Uint8 gcolor);
Uint8 get_gpixel_from_coord(SDL_Surface *surface, int x, int y);
Uint32 get_pixel_from_coord(SDL_Surface *surface, int x, int y);
Uint8 get_gpixel_from_pixel(SDL_Surface* surface, Uint32 pixel);
Uint8 get_gpixel_from_array(SDL_Surface* surface, int i);

#endif /* PIXEL_UTILS_H */
