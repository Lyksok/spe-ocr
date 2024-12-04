#ifndef CANNY_H
#define CANNY_H

#include <SDL2/SDL.h>
#include "convolution.h"
#include "../utils/converting.h"
#include <math.h>
#include <stdlib.h>

void canny_edge_detection(SDL_Surface* surface, double low_threshold, double high_threshold);

#endif