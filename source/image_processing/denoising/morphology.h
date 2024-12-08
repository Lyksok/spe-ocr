#ifndef MORPHOLOGY_H
#define MORPHOLOGY_H

#include <SDL2/SDL.h>
#include "../utils/pixel_utils.h"
#include "../parameters.h"

void convolve_int(SDL_Surface* surface, int* mask, int w);
void dilate_surface(SDL_Surface* surface, struct parameters* param);
void erode_surface(SDL_Surface* surface, struct parameters* param);

#endif /* MORPHOLOGY_H */
