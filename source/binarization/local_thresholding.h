#ifndef LOCAL_THRESHOLDING_H
#define LOCAL_THRESHOLDING_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "histograms.h"

void get_all_thresholds(SDL_Surface *surface, int **histogram);

#endif /* LOCAL_THRESHOLDING_H */
