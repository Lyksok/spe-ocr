#ifndef DENOISING_H
#define DENOISING_H

#include <SDL2/SDL.h>
#include "../utils/helper.h"
#include "../utils/pixel_utils.h"

void median_filter(SDL_Surface* surface);

#endif /* DENOISING_H */
