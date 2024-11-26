#ifndef PIXEL_UTILS_H
#define PIXEL_UTILS_H

#include <SDL2/SDL.h>

void set_gpixel_from_coord(SDL_Surface* surface, int x, int y, Uint8 gcolor);
Uint8 get_gpixel_from_coord(SDL_Surface *surface, int x, int y);
Uint32 get_pixel_from_coord(SDL_Surface *surface, int x, int y);

#endif /* PIXEL_UTILS_H */
