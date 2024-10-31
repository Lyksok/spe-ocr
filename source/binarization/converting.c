#include "converting.h"
#include "histograms.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>

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

/* Convert a single pixel of an image using a threshold and an index
*/
void convert_pixel_with_threshold(SDL_Surface* surface, int threshold,
	size_t i)
{
	SDL_LockSurface(surface);
	Uint32 pixel = ((Uint32*)surface->pixels)[i];
	Uint8 r;
	Uint8 g;
	Uint8 b;
	SDL_GetRGB(pixel, surface->format, &r, &g, &b);
	if(r<threshold)
		pixel = SDL_MapRGB(surface->format, 0, 0, 0);
	else
		pixel = SDL_MapRGB(surface->format, 255, 255, 255);
	((Uint32*)surface->pixels)[i] = pixel;
	SDL_UnlockSurface(surface);
}

/* Map a pixel to its grayscale format
 */
Uint32 to_grayscale(SDL_PixelFormat* format, Uint32 pixel)
{
    Uint8 r;
    Uint8 g;
    Uint8 b;
    SDL_GetRGB(pixel, format, &r, &g, &b);
    Uint8 max = r;
    if(g>max)
	max = g;
    else
    {
	if(b>max)
	    max = b;
    }
    Uint8 min = r;
    if(g<max)
	min = g;
    else
    {
	if(b<min)
	    min = b;
    }
    // Uint8 grayscale = 0.299*r+0.587*g+0.114*b;
    Uint8 grayscale = (min+max)/2; 
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

void binary_histogram(int** histogram, Uint32 pixel, SDL_Surface* surface)
{
    Uint8 h_index;
    Uint8 tmp1;
    Uint8 tmp2;
    SDL_GetRGB(pixel, surface->format, &h_index, &tmp1, &tmp2);
	if(h_index==0)
	{
		(*histogram)[1]+=1;
	}
	else
	{
		(*histogram)[0]+=1;
	}
}

int is_inverted(SDL_Surface* surface)
{
	int* histogram = malloc(2*sizeof(int));
	histogram[0]=0;
	histogram[1]=0;
	create_histogram_of_pixel(surface, &histogram, binary_histogram);
	if(histogram[0]>histogram[1])
	{
		free(histogram);
		return 0;
	}
	else
	{
		free(histogram);
	 	return 1;
	}
}

void invert_binarized_colors(SDL_Surface* surface)
{
	if(!is_inverted(surface))
	{
		for(int j=0; j<surface->h; j++)
		{
			for(int i=0; i<surface->w; i++)
			{
				SDL_LockSurface(surface);
				Uint32 pixel = ((Uint32*)surface->pixels)[j*surface->w+i];
				
				Uint8 h_index;
				Uint8 tmp1;
				Uint8 tmp2;
				SDL_GetRGB(pixel, surface->format, &h_index, &tmp1, &tmp2);
				if(h_index==0)
				{
					pixel = SDL_MapRGB(surface->format, 255, 255, 255);
				}
				else
				{
					pixel = SDL_MapRGB(surface->format, 0, 0, 0);
				}
				
				((Uint32*)surface->pixels)[j*surface->w+i] = pixel;

				SDL_UnlockSurface(surface);
			}
		}
	}
}
