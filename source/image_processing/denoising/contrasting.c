#include "contrasting.h"

/*
void contrast_surface(SDL_Surface* surface)
{
    Uint8 max_intensity = 0;
    Uint8 min_intensity = 255;
    for (int j = 0; j < surface->h; j++)
    {
	for (int i = 0; i < surface->w; i++)
	{
	    Uint8 pixel = get_gpixel_from_coord(surface, i, j);
	    min_intensity = min(min_intensity, pixel);
	    max_intensity = max(max_intensity, pixel);
	}
    }
    for (int j = 0; j < surface->h; j++)
    {
	for (int i = 0; i < surface->w; i++)
	{
	    Uint8 pixel = get_gpixel_from_coord(surface, i, j);
	    pixel = (Uint8)(255*((double)pixel-min_intensity)/
			((double)max_intensity-min_intensity));
	    set_gpixel_from_coord(surface, i, j, pixel);
	}
    }
}
*/

void contrast_surface(SDL_Surface* surface)
{
    
    int pixel_nb = surface->w*surface->h;
    int* freq = calloc(256, sizeof(int));
    histogram_of_pixels(surface, &freq);

    int cumulative_freq[256] = {0};
    int cumulated_sum = 0;
    for(int i=0; i<256; i++)
    {
	cumulated_sum += freq[i];
	cumulative_freq[i]=cumulated_sum;
    }
    free(freq);

    int normalized_cumulated_freq[256] = {0};
    int min_cumulated_sum = cumulative_freq[0];
    int max_cumulated_sum = cumulative_freq[255];
    for(int i=0; i<256; i++)
    {
	normalized_cumulated_freq[i]=
	    ((cumulative_freq[i]-min_cumulated_sum)*255)/
	    (max_cumulated_sum-min_cumulated_sum);
    }

    for (int j = 0; j < surface->h; j++)
    {
	for (int i = 0; i < surface->w; i++)
	{
	    Uint8 pixel = get_gpixel_from_coord(surface, i, j);
	    pixel = (int)normalized_cumulated_freq[pixel];

	    set_gpixel_from_coord(surface, i, j, pixel);
	}
    }
}
