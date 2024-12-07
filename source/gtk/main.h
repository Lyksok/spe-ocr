#ifndef MAIN_H
#define MAIN_H

#include <gtk/gtk.h>
#include <stdio.h>
#include "conversion_utils.h"
#include "image_utils.h"
#include "rotation_utils.h"
#include "pretreatment_utils.h"
#include "splash_screen.h"
#include "integration_solver.h"
#include "integration_documentation.h"
#include "my_print.h"

// CONSTANTS
#define MAX_WIDTH 600
#define MAX_HEIGHT 600
#define SAMPLE_IMAGE_PATH "images/abstract_background.jpg"

GtkWidget *init_button(const char *label, GCallback callback, gpointer data);
GtkWidget *init_menu_bar(GtkWidget *image_widget);
gboolean destroy_widget(gpointer data);
gboolean on_window_configure_event(GtkWidget *widget, GdkEvent *event, gpointer data);
// not needed static void activate(GtkApplication *app);
int main(int argc, char **argv);

#endif // MAIN_H