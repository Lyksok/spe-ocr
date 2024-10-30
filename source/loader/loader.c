#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <gtk/gtk.h>

#include "loader.h"

SDL_Surface* image = NULL;

/* Load an image to 'image'
 * Returns -1 on error else 0
 */
int load_image(char* path)
{
    image = IMG_Load(path);
    if(image == NULL)
	return -1;
    else
	return 0;
}

/* Returns image via the pointer 'img'
 * Returns -1 on error else 0
 */
int get_image(SDL_Surface* img)
{
    if(image != NULL)
    {
	img = image;
	return 0;
    }
    else
	return -1;
}

/* Saves image to file to 'path'
 * Return -1 on error else 0
 */
int save_image(SDL_Surface* surface, const char* path)
{
    if(IMG_SavePNG(surface, path))
	return -1;
    else
	return 0;
}

int get_gtk_image(GdkPixbuf* pixbuf)
{
	if(image == NULL)
		return -1;
	else
	{
		// Extract pixel data from SDL_Surface
		int width = image->w;
		int height = image->h;
		int stride = image->pitch;
		gboolean has_alpha = (image->format->BytesPerPixel == 4);

		// Create a GdkPixbuf from the SDL_Surface pixel data
		GdkPixbuf* pixbuf = gdk_pixbuf_new_from_data(
			(guchar*)image->pixels,
			has_alpha ? GDK_COLORSPACE_RGB : GDK_COLORSPACE_RGB,
			has_alpha,
			8,
			width,
			height,
			stride,
			NULL, NULL
		);

		return 0;
	}
}
