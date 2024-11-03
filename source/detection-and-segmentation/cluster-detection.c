#include <SDL2/SDL.h>
#include <err.h>
#include "structures.h"
#include "bounding-box.h"

int is_background(SDL_Surface* surface, Point coord)
{
	Uint32 pixel = ((Uint32*)surface->pixels)[coord.y*surface->w+coord.x];
    Uint8 r;
    Uint8 g;
    Uint8 b;
    SDL_GetRGB(pixel, surface->format, &r, &g, &b);
    if(r==0)
        return 1;
    else
        return 0;
}

// int nb = 0;

void flood_fill(SDL_Surface* surface, Point* p1, Point* p2,
	Point coord, int** visited)
{
    if(coord.x <= 0 || coord.y <= 0
		|| coord.x >= surface->w-1 || coord.y >= surface->h-1)
        return;
    if(is_background(surface, coord)==1
		|| (*visited)[coord.y*surface->w+coord.x]==1)
        return;

    (*visited)[coord.y*surface->w+coord.x] = 1;

    if(coord.x==p1->x)
        p1->x--;
    if(coord.y==p1->y)
        p1->y--;
    if(coord.x==p2->x)
        p2->x++;
    if(coord.y==p2->y)
        p2->y++;

    int dir_x[] = {0, 0, -1, 1};
    int dir_y[] = {-1, 1, 0, 0};
    for(size_t i=0; i<4; i++)
    {
        Point n_coord = {coord.x+dir_x[i],coord.y+dir_y[i]};
        flood_fill(surface, p1, p2, n_coord, visited);
    }
}

void get_bounding_box(SDL_Surface* surface, Point coord, BoundingBox*** boxes)
{
    // if pixel is black : go to next pixel
	Uint32 pixel = ((Uint32*)surface->pixels)[coord.y*surface->w+coord.x];
    Uint8 r;
    Uint8 g;
    Uint8 b;
    int len = surface->w*surface->h;
    SDL_GetRGB(pixel, surface->format, &r, &g, &b);
    if(r==0 || is_in_boxes(*boxes, len, coord))
    {
        return;
    }
    // if pixel is white : call recursive clustering cells
    // If pixel is on edge, return
    if(coord.x==0 || coord.y==0 || coord.x==surface->w || coord.y==surface->h)
    {
        return;
    }
    Point p1 = {coord.x-1, coord.y-1};
    Point p2 = {coord.x+1, coord.y+1};

    int* histogram = malloc(len*sizeof(int));
    for(int i=0; i<len; i++)
    {
        histogram[i]=0;
    }
    flood_fill(surface, &p1, &p2, coord, &histogram);

    // -> get boundingbox of the cluster
    BoundingBox box = {p1, p2};

    // -> check if box already exists
    if(!contains_box(box, *boxes, len))
    {
        add_box(box, boxes, len);
    }

    free(histogram);
}

void get_bounding_boxes(SDL_Surface* surface, BoundingBox*** boxes)
{
    for(int j=0; j<surface->h; j++)
    {
        for(int i=0; i<surface->w; i++)
        {
            Point coord = {i,j};
            get_bounding_box(surface, coord, boxes);
        }
    }
}
