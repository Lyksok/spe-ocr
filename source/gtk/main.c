#include "main.h"

/**
 * @brief Callback function to destroy the main window.
 */
void on_window_main_destroy(GtkWidget *widget, gpointer data)
{
  gtk_main_quit();
}

/**
 * @brief Callback function to destroy a widget.
 * @param widget The widget to be destroyed.
 * @return gboolean Always returns FALSE to stop the timeout.
 */
gboolean destroy_widget(gpointer widget)
{
  if (GTK_IS_WIDGET(widget))
  {
    GList *children, *p;
    children = gtk_container_get_children(GTK_CONTAINER(widget)); // list
    for (p = children; p != NULL; p = g_list_next(p))
    {
      gtk_widget_destroy(GTK_WIDGET(p->data));
    }
    g_list_free(children);                  // free the handy list
    gtk_widget_destroy(GTK_WIDGET(widget)); // free parent
  }
  return FALSE;
}
GtkBuilder *builder;

int main(int argc, char *argv[])
{
  GtkWidget *window;
  GtkWidget *image_widget;
  GdkPixbuf *pixbuf;

  gtk_init(&argc, &argv);

  // Load the UI file and initialize log view
  builder = gtk_builder_new_from_file("source/gtk/layout/layout.ui");
  init_log_view("source/gtk/layout/layout.ui");

  // Get the main window
  window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));

  // Get the image widget
  image_widget = GTK_WIDGET(gtk_builder_get_object(builder, "image_widget"));

  // Check if the image widget is correctly retrieved
  if (!GTK_IS_IMAGE(image_widget))
  {
    g_print("Failed to get image widget\n");
    return 1;
  }

  // Connect signals defined in the UI file
  gtk_builder_connect_signals(builder, NULL);

  // Get the screen size
  GdkScreen *screen = gdk_screen_get_default();
  gint screen_width = gdk_screen_get_width(screen);
  gint screen_height = gdk_screen_get_height(screen);

  // Set the window size to half the screen size
  gtk_window_set_default_size(GTK_WINDOW(window), screen_width / 2, screen_height);

  // Load a sample image and set it to the image widget
  pixbuf = gdk_pixbuf_new_from_file("images/Logo_OCR_WS_fanette.png", NULL);
  if (pixbuf)
  {
    process_and_display_image(image_widget, pixbuf);
    g_object_unref(pixbuf);
  }
  else
  {
    g_print("Failed to load sample image\n");
    return 1; // Exit the program if the image fails to load
  }

  // Show the main window
  gtk_widget_show_all(window);

  // Start the GTK main loop
  gtk_main();

  return 0;
}