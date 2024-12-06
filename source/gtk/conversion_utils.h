#ifndef CONVERSION_UTILS_H
#define CONVERSION_UTILS_H

#include <SDL2/SDL.h>
#include "main.h"
#include "my_print.h"

GdkPixbuf *sdl_surface_to_gdk_pixbuf(SDL_Surface *surface);
SDL_Surface *gdk_pixbuf_to_sdl_surface(GdkPixbuf *pixbuf);
GdkPixbuf *image_to_pixbuf(GtkImage *image);

#endif // CONVERSION_UTILS_H