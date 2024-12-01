#include "sdl_utils.h"

void draw_rect(SDL_Renderer *renderer, BoundingBox* box)
{
    if(SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE))
        errx(EXIT_FAILURE, "Failed to set render draw color: %s", SDL_GetError());
    SDL_Rect r;
    r.x = box->p1.x;
    r.y = box->p1.y;
    r.w = box_get_width(box);
    r.h = box_get_height(box);
    if (SDL_RenderDrawRect(renderer, &r))
        errx(EXIT_FAILURE, "Failed to fill the rectangle: %s", SDL_GetError());
}

void draw_line(SDL_Renderer *renderer, Point p1, Point p2) {
    if (SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE))
        errx(EXIT_FAILURE, "Failed to set render draw color: %s", SDL_GetError());
    if (SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y))
        errx(EXIT_FAILURE, "Failed to draw line: %s", SDL_GetError());
}
