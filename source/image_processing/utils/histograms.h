#ifndef HISTOGRAMS_H
#define HISTOGRAMS_H

#include "../detection/structures.h"
#include <SDL2/SDL.h>

void create_histogram_of_pixel(SDL_Surface *surface, int **histogram,
                               void (*func)(int **, Uint32, SDL_Surface *));
void create_histogram_of_surface(SDL_Surface *surface, int **histogram,
                                 void (*func)(int **, size_t, size_t,
                                              SDL_Surface *));
void histogram_of_pixels(SDL_Surface *surface, int **histogram);
void count_by_pixel(int **histogram, Uint32 pixel, SDL_Surface *surface);
void get_bounding_box_freq_histograms(SDL_Surface *surface, BoundingBox **boxes,
                                      int size, int **column_histo,
                                      int **row_histo);

#endif /* HISTOGRAMS_H */