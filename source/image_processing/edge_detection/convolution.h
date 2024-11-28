#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include <SDL2/SDL.h>
#include <math.h>
#include "../pixel_utils.h"

void convolve_surface(SDL_Surface* surface, double* mask, int w);

#endif /* CONVOLUTION_H */
