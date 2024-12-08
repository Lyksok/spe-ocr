#ifndef CANNY_H
#define CANNY_H

#include "../parameters.h"
#include "../utils/converting.h"
#include "../utils/pixel_utils.h"
#include "convolution.h"
#include "gaussian.h"
#include <SDL2/SDL.h>
#include <math.h>
#include <stdlib.h>

void canny_edge_detection(SDL_Surface *surface, struct parameters *param);
void non_maximum_suppression(SDL_Surface *surface, Uint8 **magnitude,
                             Uint8 **direction);
void double_threshold(SDL_Surface *surface, Uint8 **magnitude);
double *get_sobel_mask_x();
double *get_sobel_mask_y();

#endif
