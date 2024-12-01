#include "sdl_utils.h"

void draw_rect(SDL_Surface* surface, SDL_Renderer *renderer, BoundingBox* box,
        int width, int height)
{
    if(SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE))
        errx(EXIT_FAILURE, "Failed to set render draw color: %s", SDL_GetError());
    SDL_Rect r;
    int x1 = (int)ceil(((double)box->p1.x / (double)surface->w * (double)width));
    int y1 = (int)ceil(((double)box->p1.y / (double)surface->h * (double)height));
    int x2 = (int)ceil(((double)box->p2.x / (double)surface->w * (double)width));
    int y2 = (int)ceil(((double)box->p2.y / (double)surface->h * (double)height));
    r.x = x1;
    r.y = y1;
    r.w = x2-x1;
    r.h = y2-y1;
    if (SDL_RenderDrawRect(renderer, &r))
        errx(EXIT_FAILURE, "Failed to fill the rectangle: %s", SDL_GetError());
}

void draw_line(SDL_Renderer *renderer, Point p1, Point p2) {
    if (SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE))
        errx(EXIT_FAILURE, "Failed to set render draw color: %s", SDL_GetError());
    if (SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y))
        errx(EXIT_FAILURE, "Failed to draw line: %s", SDL_GetError());
}
