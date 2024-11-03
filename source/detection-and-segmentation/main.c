#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdio.h>
#include <string.h>

#include "detection.h"
#include "structures.h"
#include "segmentation.h"
#include "../binarization/binarizing.h"

int main(int argc, char **argv) {
  if (argc != 2)
    errx(EXIT_FAILURE, "Usage: image-file");

  if (SDL_Init(SDL_INIT_VIDEO) != 0)
    errx(EXIT_FAILURE, "%s", SDL_GetError());

  SDL_Window *window =
      SDL_CreateWindow("Surface_to_no_green", 0, 0, 0, 0,
                       SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
  if (window == NULL)
    errx(EXIT_FAILURE, "%s", SDL_GetError());

  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL)
    errx(EXIT_FAILURE, "%s", SDL_GetError());

  SDL_Surface *t = IMG_Load(argv[1]);
  if (t == NULL)
    errx(EXIT_FAILURE, "%s", SDL_GetError());
  SDL_Surface *surface = SDL_ConvertSurfaceFormat(t, SDL_PIXELFORMAT_RGB888, 0);
  SDL_FreeSurface(t);
  if (surface == NULL) {
    errx(EXIT_FAILURE, "%s", SDL_GetError());
  }

    // ITS HERE GUYS
    
    convert_to_grayscale(surface);
    convert_to_binarized_average(surface);
    invert_binarized_colors(surface);
    
    int word_count;
    BoundingBox** res = get_word_boxes(surface, &word_count);
    save_bounding_boxes(surface, res, word_count);
    int grid_found = 0;
    if(res==NULL)
        printf("No grid found\n");
    else
    {
        grid_found=1;
        printf("Grid found\n");
    }
//    printf("%i,%i\n", (int)((double)centroids[0].x/(double)surface->w*(double)width), 
//                                (int)((double)centroids[0].y/(double)surface->h*(double)height));
    // ITS ENDING HERE GUYS

    SDL_SetWindowSize(window,  surface->w, surface->h);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_RenderCopy(renderer,texture,NULL,NULL);
    SDL_RenderPresent(renderer);

    SDL_Event event;
    while (1)
    {
        if(grid_found)
        {
            int width;
            int height;
            SDL_GetWindowSize(window, &width, &height);
            
            for(int i=0; i<word_count; i++)
            {
                int x1 = (int)((double)res[i]->p1.x/(double)surface->w*(double)width);
                int y1 = (int)((double)res[i]->p1.y/(double)surface->h*(double)height);
                int x2 = (int)((double)res[i]->p2.x/(double)surface->w*(double)width);
                int y2 = (int)((double)res[i]->p2.y/(double)surface->h*(double)height);
                //int rect[]={boxes[i]->p1.x, boxes[i]->p1.y, boxes[i]->p2.x-boxes[i]->p1.x, boxes[i]->p2.y-boxes[i]->p1.y};
                int rect[]={x1,y1,x2-x1,y2-y1};

                draw_rect(renderer, rect);
            }
        }
        SDL_RenderPresent(renderer);
        /*
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
        for(int i=0; i<len; i++)
        {
            src[i] = (Point){(int)((double)src[i].x/(double)surface->w*(double)width),
                (int)((double)src[i].y/(double)surface->h*(double)height)};
        }
        for(int i=0; i<len; i++)
        {
            dest[i] = (Point){(int)((double)dest[i].x/(double)surface->w*(double)width),
                (int)((double)dest[i].y/(double)surface->h*(double)height)};
        }
        draw_lines(renderer, src, dest, len);
        SDL_RenderPresent(renderer);

        // END HERE
        */
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
