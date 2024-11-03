#ifndef LOADER_H
#define LOADER_H

#include <SDL2/SDL.h>
#include <gtk/gtk.h>

int load_image(char *path);
int get_image(SDL_Surface *img);
int save_image(SDL_Surface *surface, const char *path);
int get_gtk_image(GdkPixbuf *pixbuf);

#endif
