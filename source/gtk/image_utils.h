#ifndef IMAGE_UTILS_H
#define IMAGE_UTILS_H

#include "main.h"
#include "my_print.h"
#include <math.h>

void process_and_display_image(GtkWidget *image_widget, GdkPixbuf *pixbuf);
GdkPixbuf *resize_pixbuf(GdkPixbuf *pixbuf, int new_width, int new_height);
GdkPixbuf *create_pixbuf_with_alpha_borders(GdkPixbuf *pixbuf, int width,
                                            int height);
GdkPixbuf *resize_with_borders(GdkPixbuf *pixbuf);
void display_pixbuf(GtkWidget *image_widget, GdkPixbuf *pixbuf);
void on_change_image(GtkWidget *widget, gpointer data);
void *on_save_image(GtkWidget *widget, gpointer data);

#endif // IMAGE_UTILS_H