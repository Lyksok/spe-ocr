#ifndef HISTOGRAMS_H
#define HISTOGRAMS_H

#include <SDL2/SDL.h>
#include "structures.h"

void get_col_histogram(SDL_Surface* surface, int** histogram);
void get_row_histogram(SDL_Surface* surface, int** histogram);
BoundingBox*** group_by_rows(SDL_Surface* surface, BoundingBox** boxes, int len, int* n_len);
BoundingBox*** group_by_cols(SDL_Surface* surface, BoundingBox** boxes, int len, int* n_len);

#endif /* HISTOGRAMS_H */
