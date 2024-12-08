#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "convertion.h"
#include "csv.h"
#include "neural_structures.h"

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
		double *list = calloc(d * d, sizeof(int));
		SDL_Surface *letter = toSDL(argv[2]);
		SDL_to_list(letter, d * d, &list);
		for (int i = 0; i < d; i++)
		{
			for (int j = 0; j < d; j++)
			{
				printf("%f ", list[i * d + j]);
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
