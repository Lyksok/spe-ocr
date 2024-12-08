#ifndef ADAPTATIVE_H
#define ADAPTATIVE_H

#include "../parameters.h"
#include "../utils/converting.h"
#include "../utils/histograms.h"
#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>

void adaptative_thresholding(SDL_Surface *surface, struct parameters *param);

#endif /* ADAPTATIVE_H */
