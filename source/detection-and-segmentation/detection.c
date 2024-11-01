#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bounding-box.h"
#include "cluster-detection.h"
#include "structures.h"

void draw_rect(SDL_Renderer *renderer, int rect[4])
{
	if(SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE))
	    errx(EXIT_FAILURE, "Failed to set render draw color: %s", SDL_GetError());
	SDL_Rect r;
	r.x = rect[0];
	r.y = rect[1];
	r.w = rect[2];
	r.h = rect[3];
	if(SDL_RenderDrawRect(renderer, &r))
	    errx(EXIT_FAILURE, "Failed to fill the rectangle: %s", SDL_GetError());
}

void draw_rects(SDL_Renderer* renderer, int rect[][4], size_t len)
{
    for(size_t i=0; i<len; i++)
    {
        draw_rect(renderer,rect[i]);
    }
}

void draw_line(SDL_Renderer *renderer, Point p1, Point p2)
{
    if(SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE))
	errx(EXIT_FAILURE, "Failed to set render draw color: %s", SDL_GetError());
    if(SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y))
	errx(EXIT_FAILURE, "Failed to draw line: %s", SDL_GetError());
}

void draw_lines(SDL_Renderer* renderer, Point* src, Point* dest, int len)
{
    for(int i=0; i<len; i++)
    {
        draw_line(renderer, src[i], dest[i]);
    }
}


Point get_bounding_box_center(BoundingBox* b)
{
    int x = (b->p1.x+b->p2.x)/2;
    int y = (b->p1.y+b->p2.y)/2;
    Point res = {x,y};
    return res;
}

Point get_points_average(Point p1, Point p2)
{
    return (Point){(p1.x+p2.x)/2,(p1.y+p2.y)/2};
}

int ascending_bounding_box(BoundingBox* b1, BoundingBox* b2)
{
    // Define the tolerance for the grid disparity
    int tolerance = 5;
    Point p1 = get_bounding_box_center(b1);
    Point p2 = get_bounding_box_center(b2);

    if(abs(p1.y - p2.y) <= tolerance)
    {
        if(abs(p1.x - p2.x) <= tolerance)
        {
            return 1;
        }
        if(p1.x <= p2.x)
            return 1;
        else
            return 0;
    } 
    if(p1.y <= p2.y)
        return 1;
    else
        return 0;
}

/* Sort bounding boxes by ascending Y then X
*/
void sort_bounding_boxes(BoundingBox*** boxes, int len, int (*asc)(BoundingBox*, BoundingBox*))
{
    int swapped = 0;
    for(int i=0; i<len-1; i++)
    {
        swapped = 0;
        for(int j=0; j<len-i-1; j++)
        {
            BoundingBox* b1 = (*boxes)[j];
            BoundingBox* b2 = (*boxes)[j+1];
            if(!asc(b1,b2))
            {
                BoundingBox* tmp = (*boxes)[j];
                (*boxes)[j] = (*boxes)[j+1];
                (*boxes)[j+1] = tmp;
                swapped = 1;
            }
        }
        if(!swapped)
            return;
    }
}

double get_distance_points(Point p1, Point p2)
{
    return sqrt(pow(p1.x-p2.x, 2)+pow(p1.y-p2.y, 2));
}

int is_already_closest_of(Point src_box, Point dest_box, Point* src, Point* dest, int l)
{
    for(int i=0; i<l; i++)
    {
        if((are_equal_point(src_box, src[i]) && are_equal_point(dest_box, dest[i]))
            || (are_equal_point(dest_box, src[i]) && are_equal_point(src_box, dest[i])))
        {
            return 1;
        }
        else
            return 0;
    }
    return 0;
}

BoundingBox* closest_bounding_boxes(BoundingBox* box, BoundingBox** boxes, int len, Point* src, Point* dest, int l)
{
    Point p = get_bounding_box_center(box);
    // Get the first distance
    int i=0;
    while(i<len && are_equal_box(*box, *(boxes[i])))
    {
        i++;
    }
    Point q = get_bounding_box_center(boxes[i]);
    BoundingBox* closest = boxes[i];
    double min_dist = get_distance_points(p, q);

    // Get the minimal distance
    for(int j=i+1; j<len; j++)
    {
        if(!are_equal_box(*box, *(boxes[j])))
        {
            q = get_bounding_box_center(boxes[j]);
            double dist = get_distance_points(p, q);
            if(dist<min_dist && l>j)
            {
                min_dist = dist;
                closest = boxes[j];
            }
        }
    }
    return closest;
}

void get_all_links(BoundingBox** boxes, int len, Point** src, Point** dest)
{
    for(int i=0; i<len; i++)
    {
        BoundingBox* closest = closest_bounding_boxes(boxes[i], boxes, len, *src, *dest, i);
        (*src)[i] = get_bounding_box_center(boxes[i]);
        (*dest)[i] = get_bounding_box_center(closest);
    }
}

void get_char_boxes()
{
    // TODO
}

void get_word_boxes()
{
    // TODO
}

BoundingBox* get_word_list_box()
{
    // TODO
    return NULL;
}

BoundingBox* get_grid_box()
{
    // TODO
    return NULL;
}

/*
int main(int argc, char** argv)
{
    printf("Here");
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

    if(SDL_Init(SDL_INIT_VIDEO)!=0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Window* window = SDL_CreateWindow("Surface_to_no_green", 0, 0, 0, 0,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if(window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Surface* t = IMG_Load(argv[1]);
    if (t == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_Surface *surface = SDL_ConvertSurfaceFormat(t,
        SDL_PIXELFORMAT_RGB888, 0);
    SDL_FreeSurface(t);
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_SetWindowSize(window,  surface->w, surface->h);
    

	// ITS HERE GUYS
    printf("Here");
    convert_to_grayscale(surface);
    convert_to_binarized_average(surface);
    invert_binarized_colors(surface);
    int len = surface->w*surface->h;
    BoundingBox** boxes = calloc(len, sizeof(BoundingBox*));
    get_bounding_boxes(surface, &boxes);
    int min_len;
    BoundingBox** min_boxes = get_minimal(boxes, len, &min_len);
    Point* src = calloc(min_len, sizeof(Point));
    Point* dest = calloc(min_len, sizeof(Point));
    
    get_all_links(min_boxes, min_len, &src, &dest);

    // ITS ENDING HERE GUYS

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer,texture,NULL,NULL);
    SDL_RenderPresent(renderer);

    
    SDL_Event event;
    while (1)
    {

        // HERE
        int width;
        int height;
        SDL_GetWindowSize(window, &width, &height);
        
        for (int i=0; i<len; i++)
        {
            if(boxes[i]!=NULL)
            {
                int x1 = (int)((double)boxes[i]->p1.x/(double)surface->w*(double)width);
                int y1 = (int)((double)boxes[i]->p1.y/(double)surface->h*(double)height);
                int x2 = (int)((double)boxes[i]->p2.x/(double)surface->w*(double)width);
                int y2 = (int)((double)boxes[i]->p2.y/(double)surface->h*(double)height);
                //int rect[]={boxes[i]->p1.x, boxes[i]->p1.y, boxes[i]->p2.x-boxes[i]->p1.x, boxes[i]->p2.y-boxes[i]->p1.y};
                int rect[]={x1,y1,x2-x1,y2-y1};
                draw_rect(renderer, rect);
            }
        }
//        for(int i=0; i<min_len; i++)
//        {
//            src[i] = (Point){(int)((double)src[i].x/(double)surface->w*(double)width),
//                (int)((double)src[i].y/(double)surface->h*(double)height)};
//        }
//        for(int i=0; i<min_len; i++)
//        {
//            dest[i] = (Point){(int)((double)dest[i].x/(double)surface->w*(double)width),
//                (int)((double)dest[i].y/(double)surface->h*(double)height)};
//        }
        draw_lines(renderer, src, dest, min_len);
        SDL_RenderPresent(renderer);

        // END HERE

        SDL_WaitEvent(&event);
        switch (event.type)
        {
            case SDL_QUIT:
                SDL_FreeSurface(surface);
                SDL_DestroyTexture(texture);
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                free_boxes(boxes, len);
                free(boxes);
                return EXIT_SUCCESS;

            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                        SDL_RenderCopy(renderer,texture,NULL,NULL);
                        SDL_RenderPresent(renderer);
                }
                break;
        }
    }
    free_boxes(boxes, len);
    free(boxes);
    return EXIT_SUCCESS;
}
*/