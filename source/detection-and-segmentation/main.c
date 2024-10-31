#include <err.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string.h>

#include "structures.h"
#include "histograms.h"
#include "segmentation.h"

int main(int argc, char** argv)
{
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

    if(SDL_Init(SDL_INIT_VIDEO)!=0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Window* window = SDL_CreateWindow("Surface_to_no_green", 0, 0, 0, 0,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if(window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Surface* t = IMG_Load(argv[1]);
    if (t == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_Surface *surface = SDL_ConvertSurfaceFormat(t,
        SDL_PIXELFORMAT_RGB888, 0);
    SDL_FreeSurface(t);
    if (surface == NULL)
    {
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    }

        // ITS HERE GUYS
	
    	// -------------------- //
    	// for testing purposes
	Point a;
	a.x = 100;
	a.y = 100;
	Point b;
	b.x = 300;
	b.y = 300;
	BoundingBox box;
	box.p1 = a;
	box.p2 = b;
    	// -------------------- //

//	SDL_Surface *new = Cut(surface, &box);
	surface = Cut(surface, &box);
//	int IMG_SavePNG(SDL_Surface *surface, const char *file);
//	int conversion = IMG_SavePNG(surface, "yay");
//	have to put the .png at the end for it to work !
	int conv = IMG_SavePNG(surface, "yo.png");
	if (conv)
	{
		printf("Conversion to png errors\n");
	}

        // ITS ENDING HERE GUYS

    SDL_SetWindowSize(window,  surface->w, surface->h);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_RenderCopy(renderer,texture,NULL,NULL);
    SDL_RenderPresent(renderer);

    SDL_Event event;
    while (1)
    {
        SDL_WaitEvent(&event);
        switch (event.type)
        {
            case SDL_QUIT:
                SDL_FreeSurface(surface);
                SDL_DestroyTexture(texture);
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return EXIT_SUCCESS;

            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                        SDL_RenderCopy(renderer,texture,NULL,NULL);
                        SDL_RenderPresent(renderer);
                }
                break;
        }
    }
    return EXIT_SUCCESS;
}
