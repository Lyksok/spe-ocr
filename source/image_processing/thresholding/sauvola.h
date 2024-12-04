#ifndef SAUVOLA_H
#define SAUVOLA_H

#include <SDL2/SDL.h>
#include <math.h>
#include "../utils/converting.h"
#include "../utils/helper.h"
#include "../utils/pixel_utils.h"
#include "../parameters.h"

void sauvola_thresholding(SDL_Surface* surface, struct parameters* param);

#endif /* SAUVOLA_H */
