#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include "../utils/pixel_utils.h"
#include <SDL2/SDL.h>
#include <math.h>

void convolve_surface(SDL_Surface *surface, double *mask, int w);

#endif /* CONVOLUTION_H */
