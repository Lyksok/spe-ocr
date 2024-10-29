#include "histograms.h"

/* Create a histogram using an image iterating over all pixels
 * Uses an external function which has the histogram and a pixel
 * as parameters
 * Takes in parameter the result histogram and the image surface
 */
void create_histogram_of_pixel(SDL_Surface* surface, int** histogram, 
    void (*func)(int**, Uint32, SDL_Surface*))
{
    for(int j=0; j<surface->h; j++)
	{
		for(int i=0; i<surface->w; i++)
		{
			Uint32 pixel = ((Uint32*)surface->pixels)[j*surface->w+i];
            func(histogram, pixel, surface);
		}
	}
}

/* Create a histogram using an image iterating over all pixels
 * Uses an external function which has the histogram and a pixel
 * as parameters
 * Takes in parameter the result histogram and the image surface
 */
void create_histogram_of_surface(SDL_Surface* surface, void** histogram,
    void (*func)(void**, Uint32, SDL_Surface*))
{
    for(int j=0; j<surface->h; j++)
	{
		for(int i=0; i<surface->w; i++)
		{
			Uint32 pixel = ((Uint32*)surface->pixels)[j*surface->w+i];
            func(histogram, pixel, surface);
		}
	}
}

void count_by_pixel(int** histogram, Uint32 pixel, SDL_Surface* surface)
{
    Uint8 h_index;
    Uint8 tmp1;
    Uint8 tmp2;
    SDL_GetRGB(pixel, surface->format, &h_index, &tmp1, &tmp2);
    (*histogram)[h_index]+=1;
}

void histogram_of_pixels(SDL_Surface* surface, int** histogram)
{
    create_histogram_of_pixel(surface, histogram, count_by_pixel);
}