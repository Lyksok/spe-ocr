#ifndef ADAPTATIVE_H
#define ADAPTATIVE_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "../utils/histograms.h"
#include "../parameters.h"

void adaptative_thresholding(SDL_Surface* surface, struct parameters* param);

#endif /* ADAPTATIVE_H */
