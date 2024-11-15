#include "sauvola.h"

double k = 0.2; // Sensitivity
double w = 5.0; // Window size	
double R = 64.0; // Standard deviation

double get_local_mean(SDL_Surface* surface, LocalPixel* pixel)
{
    int w2 = (int)w/2;
    double sum = 0.0;
    for(int j=pixel->y-w2; j<pixel->y+w2+1; j++)
    {
	for(int i=pixel->x-w2; i<pixel->x+w2+1; i++)
	{
	    // If pixel is outside the surface => 0
	    if(j<0 || i<0 || j>=surface->h || i>=surface->w)
	    {
		sum+=255.0;
	    }
	    // If pixel is inside the surface => pix
	    else
	    {
		Uint32 pix = ((Uint32*)surface->pixels)[j*surface->w+i];
		sum+=get_grayscale_pixel(surface, pix);
	    }
	}
    }
    return sum/(w*w);
}

double get_local_std_deviation(SDL_Surface* surface, LocalPixel* pixel,
	double mean)
{
    int w2 = (int)w/2;
    int ponderate_sum = 0;
    for(int j=pixel->y-w2; j<pixel->y+w2+1; j++)
    {
	for(int i=pixel->x-w2; i<pixel->x+w2+1; i++)
	{
	    // If pixel is outside the surface => 0
	    if(j<0 || i<0 || j>=surface->h || i>=surface->w)
		ponderate_sum -= mean*mean;
	    // If pixel is inside the surface => pix
	    else
	    {
		Uint32 pix = ((Uint32*)surface->pixels)[j*surface->w+i];
		int tmp = get_grayscale_pixel(surface, pix)-mean*mean;
		ponderate_sum += tmp*tmp;
	    }
	}
    }
    return sqrt(1/(w*w)*ponderate_sum);
}

LocalPixel* get_local_pixel_info(SDL_Surface* surface, int x, int y)
{
    LocalPixel* pixel = calloc(1, sizeof(LocalPixel));
    pixel->x = x;
    pixel->y = y;

    double m = get_local_mean(surface, pixel);
    double s = get_local_std_deviation(surface, pixel, m);

    pixel->t = (int)(m*(1.0+k*(s/R-1)));
    return pixel;
}

LocalPixel** get_local_pixels_info(SDL_Surface* surface)
{
    LocalPixel** pixels = calloc(surface->w*surface->h, sizeof(LocalPixel*));
    
    for(int y=0; y<surface->h; y++)
    {
	for(int x=0; x<surface->w; x++)
	{
	    pixels[y*surface->w+x] = get_local_pixel_info(surface, x, y);
	}
    }

    return pixels;
}

void free_pixels(SDL_Surface* surface, LocalPixel** pixels)
{
    for(int i=0; i<surface->w*surface->h; i++)
	free(pixels[i]);
    free(pixels);
}

void replace_all_pixels(SDL_Surface* surface)
{
    LocalPixel** pixels = get_local_pixels_info(surface);
    for (int j = 0; j < surface->h; j++)
    {
	for (int i = 0; i < surface->w; i++)
	{
	    convert_pixel_with_threshold(surface, pixels[j*surface->w+i]->t,
		    j*surface->w+i);
	}
    }
    free_pixels(surface, pixels);
}
