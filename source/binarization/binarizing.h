#ifndef BINARIZING_H
#define BINARIZING_H

#include <SDL2/SDL.h>

void convert_to_grayscale(SDL_Surface* surface);
void convert_to_binarized(SDL_Surface* surface);

#endif /* BINARIZING_H */