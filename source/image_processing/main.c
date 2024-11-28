#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdio.h>
#include <string.h>

#include "binarization/binarizing.h"
#include "thresholding/sauvola.h"
#include "denoising/denoising.h"
#include "denoising/contrasting.h"
#include "edge_detection/gaussian.h"
#include "edge_detection/convolution.h"
#include "denoising/erosion.h"
#include "denoising/pixel_filter.h"

int main(int argc, char **argv) {
    (void)argc;
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
    errx(EXIT_FAILURE, "%s", SDL_GetError());

  SDL_Window *window =
      SDL_CreateWindow("Surface_to_no_green", 0, 0, 0, 0,
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
  if (surface == NULL) {
    errx(EXIT_FAILURE, "%s", SDL_GetError());
  }

  convert_to_grayscale(surface);
  //contrast_surface(surface);
  
 // median_filter(surface);
  int w;
  double* mask = create_gaussian_mask_5x5(&w);
  //convolve_surface(surface, mask, w);

  contrast_surface(surface);
  convolve_surface(surface, mask, w);
  sauvola_thresholding(surface);
  invert_colors(surface);
  //invert_colors(surface);
  
  free(mask);
  //erode_surface(surface);
  //convert_to_binarized_average(surface);
  //convert_to_binarized_adaptative(surface);
  //convert_to_binarized_global(surface);


  SDL_SetWindowSize(window, surface->w, surface->h);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);

  SDL_Event event;
  while (1) {
    SDL_WaitEvent(&event);
    switch (event.type) {
    case SDL_QUIT:
      SDL_QuitSubSystem(SDL_INIT_VIDEO);
      SDL_FreeSurface(surface);
      SDL_DestroyTexture(texture);
      SDL_DestroyRenderer(renderer);
      SDL_DestroyWindow(window);
      SDL_Quit();
      return EXIT_SUCCESS;

    case SDL_WINDOWEVENT:
      if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
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
  return EXIT_SUCCESS;
}
