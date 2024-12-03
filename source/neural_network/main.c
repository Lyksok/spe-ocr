#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "convertion.h"
#include "structures.h"

int main()
{
	/*
	 * FOR TESTING PURPOSES
	 * I need a SDL_Surface
	 * of a specific height and width
	 * (here 16 x 16)
	 * */
	int size = 16;
        SDL_Surface* new = SDL_CreateRGBSurface(0, size, size, 8, 0, 0, 0, 0);
	if (new == NULL)
        {
                printf("Failure to create the surface : ");
                printf("%s", SDL_GetError());
        }

	int len = size * size;
	int *list = calloc(len, sizeof(int));
	SDL_to_list(new, len, &list);
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			printf("%d ", list[i * size + j]);
		}
		printf("\n");
	}
	free(list);
	return 0;
}
