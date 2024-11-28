#ifndef CONVERTION_H
#define CONVERTION_H

#include <SDL2/SDL.h>

#include "convertion.h"
#include "structures.h"

SDL_Surface *toSDL(char *path);
void SDL_to_list(SDL_Surface *surface, int len, int **list);

#endif /* CONVERTION_H */
