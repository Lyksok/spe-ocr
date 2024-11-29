#ifndef ADAPTATIVE_H
#define ADAPTATIVE_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "../utils/histograms.h"

void get_all_thresholds(SDL_Surface *surface, int **histogram);

#endif /* ADAPTATIVE_H */
