#ifndef BINARIZING_H
#define BINARIZING_H

#include "../parameters.h"
#include "../thresholding/adaptative.h"
#include "../thresholding/global.h"
#include "../thresholding/sauvola.h"
#include "../utils/converting.h"
#include <SDL2/SDL.h>

void convert_to_grayscale(SDL_Surface *surface, struct parameters *param);
void convert_to_binarized_average(SDL_Surface *surface,
                                  struct parameters *param);
void convert_to_binarized_global(SDL_Surface *surface,
                                 struct parameters *param);
void convert_to_binarized_adaptative(SDL_Surface *surface,
                                     struct parameters *param);
void convert_to_binarized_sauvola(SDL_Surface *surface,
                                  struct parameters *param);
void invert_colors(SDL_Surface *surface);

#endif /* BINARIZING_H */
