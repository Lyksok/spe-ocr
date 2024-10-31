#include <SDL2/SDL_surface.h>
#include <SDL2/SDL.h>
#include <err.h>
#include "structures.h"
#include "bounding-box.h"

void draw_rects(SDL_Renderer *renderer, int rect[4], int len)
{
	if(SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE))
	    errx(EXIT_FAILURE, "Failed to set render draw color: %s", SDL_GetError());
	SDL_Rect r;
	r.x = rect[0];
	r.y = rect[1];
	r.w = rect[2];
	r.h = rect[3];
	if(SDL_RenderFillRect(renderer, &r))
	    errx(EXIT_FAILURE, "Failed to fill the rectangle: %s", SDL_GetError());
}

int is_in_boxes(BoundingBox** boxes, int len, Point point)
{
    for (int i=0; i<len; i++)
    {
        BoundingBox* box = boxes[i];
        // x axis
        if(box->p2.x>=point.x && box->p1.x<=point.x)
        {
            // y axis
            if(box->p2.y>=point.y && box->p1.y<=point.y)
                return 1;
        }
    }
    return 0;
}

BoundingBox get_next_bounding_box(SDL_Surface* surface, Point* coord, BoundingBox*** boxes)
{
    // if pixel is white : go to next pixel

    // if pixel is black : call recursive clustering cells (histogram with bool)
    // -> for each pixel, check if is already in box
    // -> if pixel belongs to another box, end recursion with error
    // -> else continue
    // -> get boundingbox of the cluster
    // -> check if box already exists
}
