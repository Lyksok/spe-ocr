#ifndef PIXEL_UTILS_H
#define PIXEL_UTILS_H

#include <SDL2/SDL.h>

void set_gpixel_from_coord(SDL_Surface* surface, int x, int y, Uint8 gcolor);
void set_pixelRGB_from_coord(SDL_Surface* surface, int x, int y,
	Uint8 r, Uint8 g, Uint8 b);
Uint8 get_gpixel_from_coord(SDL_Surface *surface, int x, int y);
Uint8 get_pixelR_from_coord(SDL_Surface* surface, int x, int y);
Uint8 get_pixelG_from_coord(SDL_Surface* surface, int x, int y);
Uint8 get_pixelB_from_coord(SDL_Surface* surface, int x, int y);
Uint32 get_pixel_from_coord(SDL_Surface *surface, int x, int y);
Uint8 get_gpixel_from_pixel(SDL_Surface* surface, Uint32 pixel);
Uint8 get_gpixel_from_array(SDL_Surface* surface, int i);
Uint32* get_pixels_from_surface(SDL_Surface* surface);
void set_pixels_on_surface(SDL_Surface* surface, Uint32* pixels);
int pixel_is_background_from_coord(SDL_Surface* surface, int x, int y);

#endif /* PIXEL_UTILS_H */
