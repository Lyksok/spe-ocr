#ifndef PIXEL_FILTER_H
#define PIXEL_FILTER_H

#include "../parameters.h"
#include "../utils/pixel_utils.h"
#include <SDL2/SDL.h>

void filter_small_clusters(SDL_Surface *surface, struct parameters *param);

#endif /* PIXEL_FILTER_H */
