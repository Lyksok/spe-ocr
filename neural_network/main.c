#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "convertion.h"

int main()
{
	/*
	 * FOR TESTING PURPOSES
	 * I need a SDL_Surface
	 * of a specific height and width
	 * (here 16 x 16)
	 * */
        SDL_Surface* new = SDL_CreateRGBSurface(0, 16, 16, 8, 0, 0, 0, 0);
	if (new == NULL)
        {
                printf("Failure to create the surface : ");
                printf("%s", SDL_GetError());
        }

	int len = 16 * 16;
	int *list = calloc(len, sizeof(int));
	SDL_to_list(new, len, &list);
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			printf("%d ", list[i * 16 + j]);
		}
		printf("\n");
	}
	free(list);
	return 0;
}
