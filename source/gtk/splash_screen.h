#ifndef SPLASH_SCREEN_H
#define SPLASH_SCREEN_H

#include "gtk/gtk.h"
#include "main.h"

// CONSTANTS
#define SPLASH_IMAGE_PATH "images/splash_screen_v2024_1.png"
#define SPLASH_IMAGE_WIDTH 554
#define SPLASH_IMAGE_HEIGHT 400
// FUNCTION DECLARATIONS
void show_splash_screen(GtkApplication *app);
gboolean destroy_widget(gpointer data);

#endif