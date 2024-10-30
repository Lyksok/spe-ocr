#include "splash_screen.h"

void show_splash_screen(GtkApplication *app)
{
    GtkWidget *splash_window, *splash_image, *img_splash;

    // Create splash screen window
    splash_window = gtk_application_window_new(app);
    gtk_window_set_position(GTK_WINDOW(splash_window), GTK_WIN_POS_CENTER);
    gtk_widget_set_size_request(splash_window, SPLASH_IMAGE_WIDTH, SPLASH_IMAGE_HEIGHT); // standard wizard size
    gtk_window_set_decorated(GTK_WINDOW(splash_window), FALSE);                          // remove default title bar, resize controls, etc. (called the decorations)

    // Load splash screen image
    img_splash = gtk_image_new_from_file(SPLASH_IMAGE_PATH);
    gtk_container_add(GTK_CONTAINER(splash_window), img_splash);

    gtk_window_set_keep_above(GTK_WINDOW(splash_window), TRUE); // Keep SS on top of every other windows

    // Show SS
    gtk_widget_show_all(splash_window);
    // SS exits with timeout
    g_timeout_add(3000, (GSourceFunc)gtk_widget_destroy, splash_window);
}
