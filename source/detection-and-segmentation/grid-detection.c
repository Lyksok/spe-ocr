#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

#include "bounding-box.h"
#include "../binarization/binarizing.h"
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
    free_boxes(boxes, len);
    free(boxes);

    // ITS ENDING HERE GUYS

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer,texture,NULL,NULL);
    SDL_RenderPresent(renderer);

    
    SDL_Event event;
    while (1)
    {
        SDL_WaitEvent(&event);
        switch (event.type)
        {
            case SDL_QUIT:
                SDL_FreeSurface(surface);
                SDL_DestroyTexture(texture);
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
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
    return EXIT_SUCCESS;
}
