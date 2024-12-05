#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <SDL2/SDL.h>
#include "utils/pixel_utils.h"
#include "utils/converting.h"
#include "thresholding/sauvola.h"
#include "parameters.h"

void reset_surface(SDL_Surface *surface, Uint32 *pixels);
void start_procedure(SDL_Surface *surface, struct parameters *param);

#endif /* IMAGE_PROCESSING_H */
