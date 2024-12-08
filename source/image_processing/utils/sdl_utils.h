#ifndef SDL_UTILS_H
#define SDL_UTILS_H

#include "../detection/bounding-box.h"
#include "../detection/point.h"
#include "../parameters.h"
#include "helper.h"
#include <SDL2/SDL.h>
#include <err.h>
#include <math.h>

void draw_rect2(SDL_Surface *surface, SDL_Renderer *renderer, BoundingBox *box,
                int width, int height, struct parameters *param);
void draw_line2(SDL_Renderer *renderer, Point p1, Point p2);

#endif /* SDL_UTILS_H */
