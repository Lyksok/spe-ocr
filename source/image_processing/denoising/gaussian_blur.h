#ifndef GAUSSIAN_BLUR_H
#define GAUSSIAN_BLUR_H

#include "../parameters.h"
#include "../utils/pixel_utils.h"
#include <SDL2/SDL.h>
#include <math.h>

void gaussian_blur(SDL_Surface *surface, struct parameters *param);

#endif /* GAUSSIAN_BLUR_H */
