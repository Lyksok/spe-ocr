#ifndef CONVERTION_H
#define CONVERTION_H

#include <SDL2/SDL.h>

#include "convertion.h"
#include "../gtk/conversion_utils.h"
#include "../gtk/image_utils.h"
#include "structures.h"

#define dimension 16
//const int dimension = 16;

SDL_Surface *toSDL(char *path);
void SDL_to_list(SDL_Surface *surface, int len, int **list);

#endif /* CONVERTION_H */
