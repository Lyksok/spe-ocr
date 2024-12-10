#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <err.h>
#include <stdio.h>
#include <string.h>

#include "../detection-and-segmentation/detection.h"
#include "../detection-and-segmentation/segmentation.h"
#include "binarization/binarizing.h"
#include "detection/list.h"
#include "detection/structures.h"
#include "image_processing.h"

void draw_rect(SDL_Renderer *renderer, int rect[4])
{
  if (SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE))
    errx(EXIT_FAILURE, "Failed to set render draw color: %s", SDL_GetError());
  SDL_Rect r;
  r.x = rect[0];
  r.y = rect[1];
  r.w = rect[2];
  r.h = rect[3];
  if (SDL_RenderDrawRect(renderer, &r))
    errx(EXIT_FAILURE, "Failed to fill the rectangle: %s", SDL_GetError());
}

void draw_rects(SDL_Renderer *renderer, int rect[][4], size_t len)
{
  for (size_t i = 0; i < len; i++)
  {
    draw_rect(renderer, rect[i]);
  }
}

void draw_line(SDL_Renderer *renderer, Point p1, Point p2)
{
  if (SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE))
    errx(EXIT_FAILURE, "Failed to set render draw color: %s", SDL_GetError());
  if (SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y))
    errx(EXIT_FAILURE, "Failed to draw line: %s", SDL_GetError());
}

void draw_lines(SDL_Renderer *renderer, Point *src, Point *dest, int len)
{
  for (int i = 0; i < len; i++)
  {
    draw_line(renderer, src[i], dest[i]);
  }
}

int main(int argc, char **argv)
{
  if (argc != 3)
    errx(EXIT_FAILURE, "Usage: <image-file> <chars/grid/words/list");

  if (SDL_Init(SDL_INIT_VIDEO) != 0)
    errx(EXIT_FAILURE, "%s", SDL_GetError());

  SDL_Window *window =
      SDL_CreateWindow("Detection and segmentation", 0, 0, 0, 0,
                       SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
  if (window == NULL)
    errx(EXIT_FAILURE, "%s", SDL_GetError());

  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
  if (renderer == NULL)
    errx(EXIT_FAILURE, "%s", SDL_GetError());

  SDL_Surface *t = IMG_Load(argv[1]);
  if (t == NULL)
    errx(EXIT_FAILURE, "%s", SDL_GetError());
  SDL_Surface *surface = SDL_ConvertSurfaceFormat(t, SDL_PIXELFORMAT_RGB888, 0);
  SDL_FreeSurface(t);
  if (surface == NULL)
  {
    errx(EXIT_FAILURE, "%s", SDL_GetError());
  }

  convert_to_grayscale(surface, get_parameters());
  convert_to_binarized_average(surface, get_parameters());
  invert_colors(surface);
  int number_of_characters;
  BoundingBox **characters = get_char_boxes(surface, &number_of_characters, get_parameters());
  (void)characters;
  BoundingBox *grid_box = get_grid_box(surface, get_parameters());
  BoundingBox *word_list = get_word_list_box(surface, get_parameters());
  int word_count;
  BoundingBox **words_b = get_word_boxes(surface, &word_count, get_parameters());

  int chars = 0;
  int words = 0;
  int list = 0;
  int grid = 0;

  if (strcmp("chars", argv[2]) == 0)
  {
    chars = 1;
    save_bounding_boxes(surface, characters, number_of_characters);
  }
  else if (strcmp("words", argv[2]) == 0)
  {
    words = 1;
    save_bounding_boxes(surface, words_b, word_count);
  }
  else if (strcmp("list", argv[2]) == 0)
  {
    list = 1;
    save_bounding_box(surface, word_list);
  }
  else if (strcmp("grid", argv[2]) == 0)
  {
    grid = 1;
    save_bounding_box(surface, grid_box);
  }
  else
  {
    errx(EXIT_FAILURE, "Usage: <image-file> <chars/words/list/grid>");
  }

  SDL_SetWindowSize(window, surface->w, surface->h);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);

  SDL_Event event;
  while (1)
  {
    int width;
    int height;
    SDL_GetWindowSize(window, &width, &height);

    if (words)
    {
      for (int i = 0; i < word_count; i++)
      {
        int x1 =
            (int)((double)words_b[i]->p1.x / (double)surface->w * (double)width);
        int y1 =
            (int)((double)words_b[i]->p1.y / (double)surface->h * (double)height);
        int x2 =
            (int)((double)words_b[i]->p2.x / (double)surface->w * (double)width);
        int y2 =
            (int)((double)words_b[i]->p2.y / (double)surface->h * (double)height);
        int rect[] = {x1, y1, x2 - x1, y2 - y1};
        draw_rect(renderer, rect);
      }
    }

    if (chars)
    {
      for (int i = 0; i < number_of_characters; i++)
      {
        int x1 =
            (int)((double)characters[i]->p1.x / (double)surface->w * (double)width);
        int y1 =
            (int)((double)characters[i]->p1.y / (double)surface->h * (double)height);
        int x2 =
            (int)((double)characters[i]->p2.x / (double)surface->w * (double)width);
        int y2 =
            (int)((double)characters[i]->p2.y / (double)surface->h * (double)height);
        int rect[] = {x1, y1, x2 - x1, y2 - y1};
        draw_rect(renderer, rect);
      }
    }

    if (list)
    {
      int x1 =
          (int)((double)word_list->p1.x / (double)surface->w * (double)width);
      int y1 =
          (int)((double)word_list->p1.y / (double)surface->h * (double)height);
      int x2 =
          (int)((double)word_list->p2.x / (double)surface->w * (double)width);
      int y2 =
          (int)((double)word_list->p2.y / (double)surface->h * (double)height);
      int rect[] = {x1, y1, x2 - x1, y2 - y1};
      draw_rect(renderer, rect);
    }

    if (grid)
    {
      int x1 = (int)((double)grid_box->p1.x / (double)surface->w * (double)width);
      int y1 = (int)((double)grid_box->p1.y / (double)surface->h * (double)height);
      int x2 = (int)((double)grid_box->p2.x / (double)surface->w * (double)width);
      int y2 = (int)((double)grid_box->p2.y / (double)surface->h * (double)height);
      int rect2[] = {x1, y1, x2 - x1, y2 - y1};
      draw_rect(renderer, rect2);
    }

    SDL_RenderPresent(renderer);

    SDL_WaitEvent(&event);
    switch (event.type)
    {
    case SDL_QUIT:
      SDL_QuitSubSystem(SDL_INIT_VIDEO);
      SDL_FreeSurface(surface);
      SDL_DestroyTexture(texture);
      SDL_DestroyRenderer(renderer);
      SDL_DestroyWindow(window);
      SDL_Quit();
      free(grid_box);
      free(word_list);
      for (int i = 0; i < number_of_characters; i++)
        free(characters[i]);
      free(characters);
      for (int i = 0; i < word_count; i++)
        free(words_b[i]);
      free(words_b);

      errx(EXIT_SUCCESS, "End of program");

    case SDL_WINDOWEVENT:
      if (event.window.event == SDL_WINDOWEVENT_RESIZED)
      {
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
      }
      break;
    }
  }
  SDL_QuitSubSystem(SDL_INIT_VIDEO);
  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  free(grid_box);
  free(word_list);
  for (int i = 0; i < number_of_characters; i++)
    free(characters[i]);
  free(characters);
  for (int i = 0; i < word_count; i++)
    free(words_b[i]);
  free(words_b);
  errx(EXIT_SUCCESS, "End of program");
}
