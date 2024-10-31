#include <SDL2/SDL.h>

#include "structures.h"

SDL_Surface *Cut(SDL_Surface *surface, BoundingBox *box)
{
	int x_up = box -> p1.x;
	int y_up = box -> p1.y;
	int x_down = box -> p2.x;
	int y_down = box -> p2.y;
	int w = x_down - x_up;
	int h = y_down - y_up;

	SDL_Rect area;
	area.x = x_up;
	area.y = y_up;
	area.w = w;
	area.h = h;

	SDL_Surface* new = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
	int blit = SDL_BlitSurface(surface, &area, new, NULL);
	if (blit)
	{
		printf("Failure to blit the surface\n");
	}

	return new;
}
