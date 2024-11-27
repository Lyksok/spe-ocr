#include "denoising.h"
#include <stdio.h>

int d_w = 3;

Uint8 median_value(Uint8* pixels)
{
    size_t len=d_w*d_w;
    Uint8 tmp;
    size_t j;
    // Sort the list
    for(size_t i=1; i<len; i++)
    {
        j=i;
        while(j>0 && pixels[j-1]>pixels[j])
        {
            tmp = pixels[j-1];
            pixels[j-1]=pixels[j];
            pixels[j]=tmp;
            j--;
        }
    }

    // Get the middle value
    return pixels[len/2];
}

void median_filter(SDL_Surface* surface)
{
    Uint8* local_pixels = calloc(d_w*d_w, sizeof(Uint8));
    Uint8* pixels = calloc(surface->w*surface->h, sizeof(Uint8));

    // Compute all pixel median
    for(int j=0; j<surface->h; j++)
    {
        for(int i=0; i<surface->w; i++)
        {
            // Construct surrounding pixel list
            size_t c = 0;
            for(int y=j-d_w/2; y<j+d_w/2+1; y++)
            {
                for(int x=i-d_w/2; x<i+d_w/2+1; x++)
                {
                    if(x<0 || y<0 || x>=surface->w || y>=surface->h)
                    {
                        local_pixels[c] = 255;
                        c++;
                    }
                    else
                    {
                        local_pixels[c] = get_gpixel_from_coord(surface, x, y);
                        c++;
                    }
                }
            }
            pixels[j*surface->w+i] = median_value(local_pixels);
        }
    }

    free(local_pixels);

    // Apply all changes
    for(int j=0; j<surface->h; j++)
    {
        for(int i=0; i<surface->w; i++)
        {
            set_gpixel_from_coord(surface, i, j, pixels[j*surface->w+i]);
        }
    }

    free(pixels);
}
