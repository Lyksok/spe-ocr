#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "convertion.h"
#include "structures.h"

int main(int argc, char *argv[])
{
	/*
	 * FOR TESTING PURPOSES
	 * I need a SDL_Surface
	 * of a specific height and width
	 * (here 16 x 16)
        SDL_Surface* new = SDL_CreateRGBSurface(0, size, size, 8, 0, 0, 0, 0);
	if (new == NULL)
        {
                printf("Failure to create the surface : ");
                printf("%s", SDL_GetError());
        }
	 * */
	if (argc < 3)
	{
		printf("Expected more arguments\n");
		return 1;
	}
	else if (!strcmp(argv[1], "find"))
	{
		int d = dimension;
		int *list = calloc(d * d, sizeof(int));
		SDL_Surface *letter = toSDL(argv[2]);
		SDL_to_list(letter, d * d, &list);
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
	else
	{
		// fuck it
		return 2;
	}
	return 0;
}
