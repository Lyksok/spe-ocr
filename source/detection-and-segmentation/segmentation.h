#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include <SDL2/SDL.h>

#include "structures.h"

SDL_Surface *Cut(SDL_Surface *surface, BoundingBox *box);

#endif /* SEGMENTATION_H */
