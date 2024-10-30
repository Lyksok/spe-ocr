#ifndef GTK_APP_H
#define GTK_APP_H

#include <gtk/gtk.h>

// CONSTANTS
#define IMAGE_WIDTH 600
#define IMAGE_HEIGHT 600
#define SAMPLE_IMAGE_PATH "images/abstract_background.jpg"

// FUNCTION DECLARATIONS
// TODO 30/10 update the function list !
// Image Management
GdkPixbuf *image_to_pixbuf(GtkImage *image);
GdkPixbuf *load_pixbuf(const char *file_path);
void *on_save_image(GtkWidget *widget, gpointer data);

void display_pixbuf(GtkWidget *image_widget, GdkPixbuf *pixbuf);
GdkPixbuf *resize_pixbuf(GdkPixbuf *pixbuf, int width, int height);
GdkPixbuf *create_alpha_pixbuf(int width, int height);
GdkPixbuf *calculate_white_borders(GdkPixbuf *pixbuf, int width, int height);
void on_change_image(GtkWidget *widget, gpointer data);

// GUI Management
GtkWidget *init_button(const char *label, GCallback callback, gpointer data);
GtkWidget *init_menu_bar(GtkWidget *image_widget);

// App activation
static void activate(GtkApplication *app);
int main(int argc, char **argv);

#endif