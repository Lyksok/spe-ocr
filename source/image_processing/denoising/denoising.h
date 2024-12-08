#ifndef DENOISING_H
#define DENOISING_H

#include "../parameters.h"
#include "../utils/helper.h"
#include "../utils/pixel_utils.h"
#include <SDL2/SDL.h>

void median_filter(SDL_Surface *surface, struct parameters *param);

#endif /* DENOISING_H */
