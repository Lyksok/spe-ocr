#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>

#include "histograms.h"
#include "detection.h"
#include "structures.h"
#include "bounding-box.h"

void get_col_histogram(SDL_Surface* surface, int** histogram)
{
    for (int i=0; i<surface->w; i++)
    {
        for (int j=0; j<surface->h; j++)
        {
            Uint32 pixel = ((Uint32*)surface->pixels)[j*surface->w+i];
            Uint8 r;
            Uint8 g;
            Uint8 b;
            SDL_GetRGB(pixel, surface->format, &r, &g, &b);
            if(r!=0)
                (*histogram)[i] += 1;
        }
    }
}

void get_row_histogram(SDL_Surface* surface, int** histogram)
{
    for (int j=0; j<surface->h; j++)
    {
        for (int i=0; i<surface->w; i++)
        {
            Uint32 pixel = ((Uint32*)surface->pixels)[j*surface->w+i];
            Uint8 r;
            Uint8 g;
            Uint8 b;
            SDL_GetRGB(pixel, surface->format, &r, &g, &b);
            if(r!=0)
                (*histogram)[j] += 1;
        }
    }
}

BoundingBox*** group_by_rows(SDL_Surface* surface, BoundingBox** boxes, int len, int* n_len)
{
    int avg_height = average_box_height(boxes, len);
    *n_len = surface->h/avg_height;
    BoundingBox*** res = malloc(*n_len*sizeof(BoundingBox**));

    int count;
    for(int i=0; i<*n_len; i++)
    {
        count = 0;
        BoundingBox** line = malloc(count*sizeof(BoundingBox*));
        for(int j=0; j<len; j++)
        {
            if(boxes[j]!=NULL)
            {
                Point center = get_bounding_box_center(boxes[j]);
                if(center.y>=i*avg_height && center.y<avg_height)
                {
                    count+=1;
                    line = realloc(line, count*sizeof(BoundingBox*));
                }
            }
        }
        res[i]=line;
    }
    return res;
}

BoundingBox*** group_by_cols(SDL_Surface* surface, BoundingBox** boxes, int len, int* n_len)
{
    int avg_width = average_box_width(boxes, len);
    *n_len = surface->w/avg_width;    
    BoundingBox*** res = malloc(*n_len*sizeof(BoundingBox**));

    int count;
    for(int i=0; i<*n_len; i++)
    {
        count = 0;
        BoundingBox** col = malloc(count*sizeof(BoundingBox*));
        for(int j=0; j<len; j++)
        {
            if(boxes[j]!=NULL)
            {
                Point center = get_bounding_box_center(boxes[j]);
                if(center.x>=i*avg_width && center.x<avg_width)
                {
                    count+=1;
                    col = realloc(col, count*sizeof(BoundingBox*));
                }
            }
        }
        res[i]=col;
    }
    return res;

}