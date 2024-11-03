#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdio.h>
#include <string.h>

#include "detection.h"
#include "structures.h"
#include "segmentation.h"
#include "../binarization/binarizing.h"

int main(int argc, char **argv) {
  if (argc != 3 && argc != 4)
    errx(EXIT_FAILURE,
  "Usage: <image-file> <detection/segmentation> <chars/words/list/grid>");

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
  SDL_Surface *surface=SDL_ConvertSurfaceFormat(t, SDL_PIXELFORMAT_RGB888, 0);
  SDL_FreeSurface(t);
  if (surface == NULL) {
    errx(EXIT_FAILURE, "%s", SDL_GetError());
  }

  convert_to_grayscale(surface);
  convert_to_binarized_average(surface);
  invert_colors(surface);

  int is_detection = 0;
  if(strcmp(argv[2], "detection")==0)
  {
    is_detection = 1;
  }
  else if (strcmp(argv[2], "segmentation")==0)
  {
    BoundingBox* grid_box = get_grid_box(surface);
    save_bounding_box(surface, grid_box);
    return EXIT_SUCCESS;
  }
  else
    errx(EXIT_FAILURE, "Usage: <image-file> <detection/segmentation>");

  if(!is_detection)
    return EXIT_SUCCESS;

  int chars = 0;
  int words = 0;
  int list = 0;
  int grid = 0;

  if(strcmp(argv[3], "chars")==0)
    chars = 1;
  else if(strcmp(argv[3], "words")==0)
    words = 1;
  else if(strcmp(argv[3], "list")==0)
    list = 1;
  else if(strcmp(argv[3], "grid")==0)
    grid = 1;

  int number_of_characters;
  BoundingBox** characters = get_char_boxes(surface, &number_of_characters);
  BoundingBox* grid_box = get_grid_box(surface);
  BoundingBox* word_list = get_word_list_box(surface);
  int word_count;
  BoundingBox** words_b = get_word_boxes(surface, &word_count);

  SDL_SetWindowSize(window,  surface->w, surface->h);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_RenderCopy(renderer,texture,NULL,NULL);
  SDL_RenderPresent(renderer);

  SDL_Event event;
  while (1)
  {
    int width;
    int height;
    SDL_GetWindowSize(window, &width, &height);

    for(int i=0; i<word_count; i++)
    {
    int x1 =(int)((double)words_b[i]->p1.x/(double)surface->w*(double)width);
    int y1 =(int)((double)words_b[i]->p1.y/(double)surface->h*(double)height);
    int x2 =(int)((double)words_b[i]->p2.x/(double)surface->w*(double)width);
    int y2 =(int)((double)words_b[i]->p2.y/(double)surface->h*(double)height);
    int rect[]={x1,y1,x2-x1,y2-y1};
    draw_rect(renderer, rect);
    }
    int x1 = (int)((double)word_list->p1.x/(double)surface->w*(double)width);
    int y1 = (int)((double)word_list->p1.y/(double)surface->h*(double)height);
    int x2 = (int)((double)word_list->p2.x/(double)surface->w*(double)width);
    int y2 = (int)((double)word_list->p2.y/(double)surface->h*(double)height);
    int rect[]={x1,y1,x2-x1,y2-y1};
    draw_rect(renderer, rect);

    x1 = (int)((double)grid_box->p1.x/(double)surface->w*(double)width);
    y1 = (int)((double)grid_box->p1.y/(double)surface->h*(double)height);
    x2 = (int)((double)grid_box->p2.x/(double)surface->w*(double)width);
    y2 = (int)((double)grid_box->p2.y/(double)surface->h*(double)height);
    int rect2[]={x1,y1,x2-x1,y2-y1};
    draw_rect(renderer, rect2);

    SDL_RenderPresent(renderer);

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
