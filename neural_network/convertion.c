#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "convertion.h"

// RGB WHITE = 255, 255, 255
// RGB BLACK = 0, 0, 0

SDL_Surface *toSDL(char *path)
{
	SDL_Surface *new = IMG_Load(path);
	if (new == NULL)
	{
		printf("Failure to create the surface");
	}
	return new;
}

void SDL_to_list(SDL_Surface *surface, int **list, int len)
{
	for (int i = 0; i < len; i++)
	{
		printf("%d ", *list[i]);
	}
	
	SDL_FreeSurface(surface);
	return;
}
