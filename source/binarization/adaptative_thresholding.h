#ifndef ADAPTATIVE_THRESHOLDING_H
#define ADAPTATIVE_THRESHOLDING_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "histograms.h"

Uint8 get_grayscale_pixel(SDL_Surface *surface, Uint32 pixel);
void get_all_thresholds(SDL_Surface *surface, int **histogram);

#endif /* ADAPTATIVE_THRESHOLDING_H */
