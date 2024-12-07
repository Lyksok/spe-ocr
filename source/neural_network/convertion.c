#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>

#include "convertion.h"
#include "../gtk/conversion_utils.h"
#include "../gtk/image_utils.h"
#include "neural_structures.h"

// RGB WHITE = 255, 255, 255
// RGB BLACK = 0, 0, 0

/*
 * path : a path to a png image
 * returns : a surface from the image
 * */
SDL_Surface *toSDL(char *path)
{
	SDL_Surface *temp = IMG_Load(path);
	if (temp == NULL)
	{
		printf("Failure to create the surface : ");
		printf("%s", SDL_GetError());
	}

	/*
	 * GdkPixbuf *sdl_surface_to_gdk_pixbuf(SDL_Surface *surface);
	 * GdkPixbuf *resize_pixbuf(GdkPixbuf *pixbuf,
				int new_width,
				int new_height);
	 * SDL_Surface *gdk_pixbuf_to_sdl_surface(GdkPixbuf *pixbuf);
	 * */
	int d = dimension;
	GdkPixbuf *pix = sdl_surface_to_gdk_pixbuf(temp);
	pix = resize_pixbuf(pix, d, d);
	temp = gdk_pixbuf_to_sdl_surface(pix);

	SDL_Surface *new;
	new = SDL_ConvertSurfaceFormat(temp, SDL_PIXELFORMAT_INDEX8, 0);
	/*
	 * SDL_PIXELFORMAT_INDEX8 :
	 * best for greyscaled images
	 * most memory-efficient format for this
	 * */
	SDL_FreeSurface(temp);
	if (new == NULL)
	{
		printf("Failure to create the surface : ");
		printf("%s", SDL_GetError());
	}
	SDL_ClearError();

	return new;
}

/*
 * surface : a surface of black and white pixels
 * --> format is SDL_PIXELFORMAT_INDEX8
 * list : a pointer to a list, to be filled with 0 and 1
 * len : the length of the list
 * returns : build in place the list
 * TODO
 * check if it's actually working
 * currently tested with a new surface
 * no idea wether said surface is black or white
 * thus need further ckecks
 * */
void SDL_to_list(SDL_Surface *surface, int len, double **list)
{
	SDL_LockSurface(surface);
	Uint8 *pixels = (Uint8 *)surface->pixels;
	for (int i = 0; i < len; i++)
	{
		Uint8 pixel = pixels[i];
		Uint8 r;
		Uint8 g;
		Uint8 b;
		SDL_GetRGB(pixel, surface->format, &r, &g, &b);
		// we take a surface with only black and white
		// if one of the parameter is != 0, then it has to be white
		(*list)[i] = r ? 1 : 0;
	}
	SDL_UnlockSurface(surface);
	SDL_FreeSurface(surface);
	return;
}
