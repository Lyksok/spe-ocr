#ifndef SDL_UTILS_H
#define SDL_UTILS_H

#include <SDL2/SDL.h>
#include <err.h>
#include "../detection/bounding-box.h"
#include "../detection/point.h"

void draw_rect(SDL_Renderer *renderer, BoundingBox* box);
void draw_line(SDL_Renderer *renderer, Point p1, Point p2);

#endif /* SDL_UTILS_H */
