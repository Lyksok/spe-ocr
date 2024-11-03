#include <SDL2/SDL.h>
#include <stdio.h>

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

BoundingBox*** group_by_rows(SDL_Surface* surface, BoundingBox** boxes,
	int len, int* n_len)
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

BoundingBox*** group_by_cols(SDL_Surface* surface, BoundingBox** boxes,
	int len, int* n_len)
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

int* get_row_box_histogram(SDL_Surface* surface, BoundingBox** boxes, int len)
{
    int* histogram = malloc(surface->h*sizeof(int));
    for(int i=0; i<surface->h; i++)
    {
        histogram[i]=0;
    }
    for (int j=0; j<surface->h; j++)
    {
        for(int i=0; i<len; i++)
        {
            Point point = {get_bounding_box_center(boxes[i]).x,j};
            if(is_in_box(boxes[i], point))
            {
                histogram[j]+=1;
            }
        }
    }
    return histogram;
}

int* get_col_box_histogram(SDL_Surface* surface, BoundingBox** boxes, int len)
{
    int* histogram = malloc(surface->w*sizeof(int));
    for(int i=0; i<surface->w; i++)
    {
        histogram[i]=0;
    }
    for (int j=0; j<surface->w; j++)
    {
        for(int i=0; i<len; i++)
        {
            Point point = {j,get_bounding_box_center(boxes[i]).y};
            if(is_in_box(boxes[i], point))
            {
                histogram[j]+=1;
            }
        }
    }
    return histogram;
}

void print_int_histogram(int* histogram, int len)
{
    for(int i=0; i<len; i++)
    {
        printf("%i ", histogram[i]);
    }
}

int get_max(int* histogram, int len)
{
    int max = histogram[0];
    for(int i=1; i<len; i++)
    {
        if(histogram[i]>max)
            max = histogram[i];
    }
    return max;
}

int get_max_by_index(int* histogram, int len)
{
    int max = histogram[0];
    int max_index = 0;
    for(int i=1; i<len; i++)
    {
        if(histogram[i]>max || max_index==0)
        {
            max = histogram[i];
            max_index = i;
        }
    }
    return max_index;
}

int get_most_frequent(int* histogram, int len)
{
    int max = get_max(histogram, len);
    int* range = malloc((max+1)*sizeof(int));
    for(int i=0; i<=max; i++)
    {
        range[i]=0;
    }
    for(int i=0; i<len; i++)
    {
        range[histogram[i]]+=1;
    }
    int most_frequent = get_max_by_index(range, max+1);
    free(range);
    return most_frequent;
}

int* get_row_bounded_box_histogram(BoundingBox** boxes, BoundingBox* box,
	int len)
{
    int* histogram = malloc(box->p2.y*sizeof(int));
    for(int i=0; i<box->p2.y; i++)
    {
        histogram[i]=0;
    }

    for(int j=box->p1.y; j<box->p2.y; j++)
    {
        for(int i=0; i<len; i++)
        {
            Point point = {get_bounding_box_center(boxes[i]).x,j};
            if(is_in_box(boxes[i], point))
            {
                histogram[j]+=1;
            }
        }
    }
    return histogram;
}
