#include "morphology.h"

void convolve_int(SDL_Surface* surface, int* mask, int w)
{
    Uint8* pixels = get_gpixels_from_surface(surface);
    for(int j=0; j<surface->h; j++)
    {
        for(int i=0; i<surface->w; i++)
        {
            int sum=0;
            for(int y=-w/2; y<=w/2; y++)
            {
                for(int x=-w/2; x<=w/2; x++)
                {
                    Uint8 pixel;
                    if(i+x<0 || i+x>=surface->w || j+y<0 || j+y>=surface->h)
                    {
                        pixel = 255;
                    }
                    else
                    {
                        pixel = pixels[(j+y)*surface->w+i+w];
                    }
                    sum += mask[(y+w/2)*w+x+w/2]*pixel;
                }
            }
            set_gpixel_from_coord(surface, i, j, sum/(w*w));
        }
    }
    free(pixels);
}

static int dilation_mask_cross[] = {0, 1, 0, 1, 1, 1, 0, 1, 0};
static int dilation_mask_full[9] = {1};
static int w=3;

void dilate_surface(SDL_Surface* surface, struct parameters* param)
{
    int* mask = dilation_mask_cross;
    if(param->dilation_m==0)
        mask = dilation_mask_cross;
    else if(param->dilation_m==1)
        mask = dilation_mask_full;

    Uint8* pixels = get_gpixels_from_surface(surface);
    for(int j=0; j<surface->h; j++)
    {
        for(int i=0; i<surface->w; i++)
        {
            // If one is foreground, fill the pixel with foreground
            int in_mask=0;
            for(int y=-w/2; y<=w/2 && !in_mask; y++)
            {
                for(int x=-w/2; x<=w/2 && !in_mask; x++)
                {
                    if(!mask[(y+w/2)*w+x+w/2])
                        continue;
                    if(i+x<0 || i+x>=surface->w || j+y<0 || j+y>=surface->h)
                        continue;
                    if(pixels[(j+y)*surface->w+i+x])
                        in_mask = 1;
                }
            }
            if(in_mask)
                set_gpixel_from_coord(surface, i, j, 255);
        }
    }
    free(pixels);
}

static int erosion_mask_cross[] = {0, 1, 0, 1, 1, 1, 0, 1, 0};
static int erosion_mask_full[9] = {1};

void erode_surface(SDL_Surface* surface, struct parameters* param)
{
    int* mask = erosion_mask_cross;
    if(param->erosion_m==0)
        mask = erosion_mask_cross;
    else if(param->erosion_m==1)
        mask = erosion_mask_full;

    Uint8* pixels = get_gpixels_from_surface(surface);
    for(int j=0; j<surface->h; j++)
    {
        for(int i=0; i<surface->w; i++)
        {
            // If one is not foreground, fill the pixel with background
            int in_mask=1;
            for(int y=-w/2; y<=w/2 && in_mask; y++)
            {
                for(int x=-w/2; x<=w/2 && in_mask; x++)
                {
                    if(!mask[(y+w/2)*w+x+w/2])
                        continue;
                    if(i+x<0 || i+x>=surface->w || j+y<0 || j+y>=surface->h)
                        continue;
                    if(!pixels[(j+y)*surface->w+i+x])
                        in_mask = 0;
                }
            }
            if(!in_mask)
                set_gpixel_from_coord(surface, i, j, 0);
        }
    }
}
