#ifndef GTK_APP_H
#define GTK_APP_H

// INCLUDE DEPENDENCIES
#include <gtk/gtk.h>
#include "splash_screen.h"
#include "../binarization/binarizing.h"

// CONSTANTS
#define PI 3.1415926535897932
#define DEFAULT_LEFT_ANGLE -5.0 // Default left rotation angle
#define DEFAULT_RIGHT_ANGLE 5.0 // Default right rotation angle
#define IMAGE_WIDTH 1000
#define IMAGE_HEIGHT 1000
#define SAMPLE_IMAGE_PATH "images/abstract_background.jpg"

// FUNCTION DECLARATIONS
// Callback functions begin with "on_"
// All function names are in snake_case
GdkPixbuf *image_to_pixbuf(GtkImage *image);
GdkPixbuf *load_pixbuf(const char *file_path);
void *on_save_image(GtkWidget *widget, gpointer data);
GtkImage *init_image_widget(GtkWidget *parent, const char *sample_image_path);
void display_pixbuf(GtkWidget *image_widget, GdkPixbuf *pixbuf);
GdkPixbuf *resize_pixbuf(GdkPixbuf *pixbuf, int width, int height);
GdkPixbuf *create_alpha_pixbuf(int width, int height);
GdkPixbuf *calculate_borders(GdkPixbuf *pixbuf, int width, int height);
void on_change_image(GtkWidget *widget, gpointer data);
GdkPixbuf *sdl_surface_to_gdk_pixbuf(SDL_Surface *surface);
SDL_Surface *gdk_pixbuf_to_sdl_surface(GdkPixbuf *pixbuf);
void on_grayscale_clicked(GtkWidget *widget, gpointer data);
void on_binarize_clicked(GtkWidget *widget, gpointer data);
GdkPixbuf *rotate_pixbuf(GdkPixbuf *pixbuf, double angle);
void on_rotate_left_clicked(GtkWidget *widget, gpointer data);
void on_rotate_right_clicked(GtkWidget *widget, gpointer data);
void on_rotate_clicked(GtkWidget *widget, gpointer data);
void on_angle_entry_activate(GtkEntry *entry, gpointer data);

GtkWidget *init_button(const char *label, GCallback callback, gpointer data);
GtkWidget *init_menu_bar(GtkWidget *image_widget);

static void activate(GtkApplication *app);
int main(int argc, char **argv);

#endif