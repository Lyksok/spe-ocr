#ifndef HISTOGRAMS_H
#define HISTOGRAMS_H

#include <SDL2/SDL.h>

void create_histogram_of_pixel(SDL_Surface* surface, int** histogram,
    void (*func)(int**, Uint32, SDL_Surface*));
void create_histogram_of_surface(SDL_Surface* surface, int** histogram,
    void (*func)(int**, size_t, size_t, SDL_Surface*));
void histogram_of_pixels(SDL_Surface* surface, int** histogram);

#endif /* HISTOGRAMS_H */
