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
