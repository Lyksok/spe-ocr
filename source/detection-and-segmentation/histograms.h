#ifndef HISTOGRAMS_H
#define HISTOGRAMS_H

#include <SDL2/SDL.h>

void get_col_histogram(SDL_Surface* surface, int** histogram);
void get_row_histogram(SDL_Surface* surface, int** histogram);

#endif /* HISTOGRAMS_H */
