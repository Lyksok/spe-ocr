#ifndef BINARIZING_H
#define BINARIZING_H

#include <SDL2/SDL.h>
#include "../utils/converting.h"
#include "../thresholding/global.h"
#include "../thresholding/adaptative.h"
#include "../thresholding/sauvola.h"

void convert_to_grayscale(SDL_Surface *surface);
void convert_to_binarized_average(SDL_Surface *surface);
void convert_to_binarized_global(SDL_Surface *surface);
void convert_to_binarized_adaptative(SDL_Surface *surface);
void convert_to_binarized_sauvola(SDL_Surface *surface);
void invert_colors(SDL_Surface *surface);

#endif /* BINARIZING_H */
