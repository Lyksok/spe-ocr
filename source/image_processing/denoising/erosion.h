#ifndef EROSION_H
#define EROSION_H 

#include <SDL2/SDL.h>
#include "../utils/pixel_utils.h"
#include "parameters.h"

void erode_surface(SDL_Surface* surface, struct parameters* param);

#endif /* EROSION_H */
