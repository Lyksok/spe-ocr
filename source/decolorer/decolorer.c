#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>

#include "decolorer.h"
#include "../loader/loader.h"

/* Function to change a pixel with new r, g and b values
 * Returns a pixel
 */
Uint32 change_pixel(SDL_PixelFormat *format,
	Uint8 nr, Uint8 ng, Uint8 nb)
{
    return SDL_MapRGB(format, nr, ng, nb);
}

/* Function to convert a pixel into its grayscale format
 * Return a pixel
 */
Uint32 to_grayscale(SDL_PixelFormat* format, Uint32 pixel)
{
    Uint8 r;
    Uint8 g;
    Uint8 b;
    SDL_GetRGB(pixel, surface->format, &r, &g, &b);
    Uint8 grayscale = 0.299*r+0.587*g+0.114*b;
    return SDL_MapRGB(format, grayscale, grayscale, grayscale);
}

void image_to_grayscale(SDL_Surface* surface)
{
    for(int j=0; j<g_image->w; j++)
	for(int i=0; i<g_image->h; i++)
	{
	    SDL_LockSurface(surface);
	    Uint32 pixel = ((Uint32*)surface->pixels)[j*surface->w+i];

	    pixel = to_grayscale(surface->format, pixel);
	    ((Uint32*)surface->pixels)[j*surface->w+i] = pixel;

	    SDL_UnlockSurface(surface);
	}
}

/* Create a histogram using a grayscale image
 * Takes in parameter the result histogram and the image surface
 */
void get_histogram(SDL_Surface* g_image, int[256] histogram)
{
    for(size_t i=0; i<256; i++)
	histogram[i]=0;
    for(int j=0; j<g_image->w; j++)
	for(int i=0; i<g_image->h; i++)
	{
	    Uint32 pixel = ((Uint32*)g_image->pixels)[j*g_image->w+i];
	    size_t h_index;
	    SDL_GetRGB(pixel, g_image->format, &h_index, &h_index, &h_index);
	    histogram[h_index]+=1;
	}
}
