#ifndef BINARIZING_H
#define BINARIZING_H

#include <SDL2/SDL.h>
#include "converting.h"
#include "global_thresholding.h"
#include "local_thresholding.h"

void convert_to_grayscale(SDL_Surface* surface);
void convert_to_binarized_average(SDL_Surface* surface);
void convert_to_binarized_global(SDL_Surface* surface);
void convert_to_binarized_local(SDL_Surface* surface);
void invert_colors(SDL_Surface* surface);

#endif /* BINARIZING_H */
