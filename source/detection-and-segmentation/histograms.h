#ifndef HISTOGRAMS_H
#define HISTOGRAMS_H

#include <SDL2/SDL.h>
#include "structures.h"

void get_col_histogram(SDL_Surface* surface, int** histogram);
void get_row_histogram(SDL_Surface* surface, int** histogram);
BoundingBox*** group_by_rows(SDL_Surface* surface, BoundingBox** boxes, int len, int* n_len);
BoundingBox*** group_by_cols(SDL_Surface* surface, BoundingBox** boxes, int len, int* n_len);
int* get_row_box_histogram(SDL_Surface* surface, BoundingBox** boxes, int len);
int* get_col_box_histogram(SDL_Surface* surface, BoundingBox** boxes, int len);
void print_int_histogram(int* histogram, int len);

int get_most_frequent(int* histogram, int len);

#endif /* HISTOGRAMS_H */
