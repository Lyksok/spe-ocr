#ifndef SAUVOLA_H
#define SAUVOLA_H

#include <SDL2/SDL.h>
#include "math.h"
#include "../detection-and-segmentation/structures.h"
#include "converting.h"
#include "adaptative_thresholding.h"

typedef struct LocalPixel
{
    int x;
    int y;
    int t;
} LocalPixel;
void replace_all_pixels(SDL_Surface* surface);

#endif /* SAUVOLA_H */
