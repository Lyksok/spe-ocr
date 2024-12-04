#ifndef PIXEL_FILTER_H
#define PIXEL_FILTER_H

#include <SDL2/SDL.h>
#include "../utils/pixel_utils.h"
#include "../parameters.h"

void filter_small_clusters(SDL_Surface* surface, struct parameters* param);

#endif /* PIXEL_FILTER_H */
