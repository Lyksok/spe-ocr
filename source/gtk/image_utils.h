#ifndef IMAGE_UTILS_H
#define IMAGE_UTILS_H

#include "main.h"

GdkPixbuf *load_pixbuf(const char *file_path);
void display_pixbuf(GtkWidget *image_widget, GdkPixbuf *pixbuf);
GdkPixbuf *resize_pixbuf(GdkPixbuf *pixbuf, int width, int height);
GdkPixbuf *create_alpha_pixbuf(int width, int height);
GdkPixbuf *calculate_borders(GdkPixbuf *pixbuf, int width, int height);
void on_change_image(GtkWidget *widget, gpointer data);
void *on_save_image(GtkWidget *widget, gpointer data);
#endif // IMAGE_UTILS_H