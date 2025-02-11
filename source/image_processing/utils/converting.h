#ifndef CONVERTING_H
#define CONVERTING_H

#include "../detection/bounding-box.h"
#include "../parameters.h"
#include "histograms.h"
#include "pixel_utils.h"
#include <SDL2/SDL.h>

int is_inverted(SDL_Surface *surface);
void image_to_grayscale(SDL_Surface *surface, struct parameters *param);
void image_to_binarized(SDL_Surface *surface, size_t threshold);
void convert_image(SDL_Surface *surface,
                   Uint32 (*func)(SDL_PixelFormat *, Uint32));
void convert_image_with_threshold(SDL_Surface *surface, size_t threshold,
                                  Uint32 (*func)(SDL_PixelFormat *, Uint32,
                                                 size_t));
void convert_pixel_with_threshold(SDL_Surface *surface, int threshold,
                                  size_t i);
void invert_binarized_colors(SDL_Surface *surface);
void remove_box_pixels(SDL_Surface *surface, BoundingBox *box);

#endif /* CONVERTING_H */
