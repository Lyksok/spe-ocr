#include "pixel_utils.h"

void set_gpixel_from_coord(SDL_Surface* surface, int x, int y, Uint8 gcolor)
{
    SDL_LockSurface(surface);
    
    Uint32 pixel = SDL_MapRGB(surface->format, gcolor, gcolor, gcolor);

    ((Uint32*)surface->pixels)[y*surface->w+x] = pixel;

    SDL_UnlockSurface(surface);
}

Uint8 get_gpixel_from_coord(SDL_Surface *surface, int x, int y)
{
    Uint32 pixel = ((Uint32*)surface->pixels)[y*surface->w+x];
    Uint8 res;
    Uint8 g;
    Uint8 b;
    SDL_GetRGB(pixel, surface->format, &res, &g, &b);
    return res;
}

Uint32 get_pixel_from_coord(SDL_Surface *surface, int x, int y)
{
    Uint32 res = ((Uint32*)surface->pixels)[y*surface->w+x];
    return res;
}

Uint8 get_gpixel_from_pixel(SDL_Surface* surface, Uint32 pixel)
{
    Uint8 res;
    Uint8 g;
    Uint8 b;
    SDL_GetRGB(pixel, surface->format, &res, &g, &b);
    return res;
}

Uint8 get_gpixel_from_array(SDL_Surface* surface, int i)
{
    return get_gpixel_from_coord(surface, i, 0);
}

Uint32* get_pixels_from_surface(SDL_Surface* surface)
{
    return (Uint32*)surface->pixels;
}

void set_pixels_on_surface(SDL_Surface* surface, Uint32* pixels)
{
    surface->pixels = pixels;
}
