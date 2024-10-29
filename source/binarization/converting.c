#include "converting.h"

/* Convert an image using an external function to reassign
 * each pixel with a different value based on the function
 */
void convert_image(SDL_Surface* surface, 
	Uint32 (*func)(SDL_PixelFormat*, Uint32))
{
    for(int j=0; j<surface->h; j++)
	{
		for(int i=0; i<surface->w; i++)
		{
			SDL_LockSurface(surface);
			Uint32 pixel = ((Uint32*)surface->pixels)[j*surface->w+i];

			pixel = func(surface->format, pixel);
			((Uint32*)surface->pixels)[j*surface->w+i] = pixel;

			SDL_UnlockSurface(surface);
		}
	}
}

/* Convert an image using an external function to reassign
 * each pixel with a different value based on the function
 * Add a threshold in parameters
 */
void convert_image_with_threshold(SDL_Surface* surface, size_t threshold, 
	Uint32 (*func)(SDL_PixelFormat*, Uint32, size_t))
{
    for(int j=0; j<surface->h; j++)
	{
		for(int i=0; i<surface->w; i++)
		{
			SDL_LockSurface(surface);
			Uint32 pixel = ((Uint32*)surface->pixels)[j*surface->w+i];

			pixel = func(surface->format, pixel, threshold);
			((Uint32*)surface->pixels)[j*surface->w+i] = pixel;

			SDL_UnlockSurface(surface);
		}
	}
}


/* Map a pixel to its grayscale format
 */
Uint32 to_grayscale(SDL_PixelFormat* format, Uint32 pixel)
{
    Uint8 r;
    Uint8 g;
    Uint8 b;
    SDL_GetRGB(pixel, format, &r, &g, &b);
    Uint8 grayscale = 0.299*r+0.587*g+0.114*b;
    return SDL_MapRGB(format, grayscale, grayscale, grayscale);
}

/* Convert in place an image using its surface to a grayscale image
 */
void image_to_grayscale(SDL_Surface* surface)
{
	convert_image(surface, to_grayscale);
}

/* Map a grayscale pixel to its binarized format based
	on a threshold
 */
Uint32 to_binarized(SDL_PixelFormat* format, Uint32 pixel, size_t threshold)
{
	Uint8 r;
	Uint8 g;
	Uint8 b;
	SDL_GetRGB(pixel, format, &r, &g, &b);
	if(r<threshold)
		return SDL_MapRGB(format, 0, 0, 0);
	else
		return SDL_MapRGB(format, 255, 255, 255);
}

/* Convert in place an image using its surface to a grayscale image
 */
void image_to_binarized(SDL_Surface* surface, size_t threshold)
{
	convert_image_with_threshold(surface, threshold, to_binarized);
}
