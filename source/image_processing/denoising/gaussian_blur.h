#ifndef GAUSSIAN_BLUR_H
#define GAUSSIAN_BLUR_H

#include <SDL2/SDL.h>
#include <math.h>
#include "../parameters.h"
#include "../utils/pixel_utils.h"

void gaussian_blur(SDL_Surface* surface, struct parameters* param);

#endif /* GAUSSIAN_BLUR_H */
