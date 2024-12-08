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

/**
 * @brief Callback function to handle window resize events.
 */
gboolean on_window_configure_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
  GdkScreen *screen = gdk_screen_get_default();
  gint screen_width = gdk_screen_get_width(screen);
  gint screen_height = gdk_screen_get_height(screen);
  gtk_window_set_default_size(GTK_WINDOW(widget), screen_width / 2, screen_height);
  return FALSE;
}

GtkBuilder *builder;

int main(int argc, char *argv[])
{

  /**
   * TODO 8/12 rotation old was working
   *
   * // Manage entry buttons for the rotation
  // Update angle values according to user input
  GtkWidget *left_angle_entry = gtk_entry_new();
  GtkWidget *right_angle_entry = gtk_entry_new();
  char *left_angle_text = g_strdup_printf("%.2f", DEFAULT_LEFT_ANGLE);
  gtk_entry_set_text(GTK_ENTRY(left_angle_entry), left_angle_text);
  g_free(left_angle_text);

  char *right_angle_text = g_strdup_printf("%.2f", DEFAULT_RIGHT_ANGLE);
  gtk_entry_set_text(GTK_ENTRY(right_angle_entry), right_angle_text);
  g_free(right_angle_text);

  g_signal_connect(left_angle_entry, "activate", G_CALLBACK(on_angle_entry_activate), &left_angle);   // updates when the user presses enter
  g_signal_connect(right_angle_entry, "activate", G_CALLBACK(on_angle_entry_activate), &right_angle); // updates when the user presses enter
   */
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
    my_print("gtk/main.c : Failed to get image widget\n");
    return 1;
  }

  // Connect signals defined in the UI file
  gtk_builder_connect_signals(builder, NULL);
  // Connect rotation signals
  GtkWidget *left_angle_enter = GTK_WIDGET(gtk_builder_get_object(builder, "left_angle_enter"));
  g_signal_connect(left_angle_enter, "clicked", G_CALLBACK(on_rotate_left_clicked), image_widget);

  GtkWidget *right_angle_enter = GTK_WIDGET(gtk_builder_get_object(builder, "right_angle_enter"));
  g_signal_connect(right_angle_enter, "clicked", G_CALLBACK(on_rotate_right_clicked), image_widget);

  GtkWidget *run_button = GTK_WIDGET(gtk_builder_get_object(builder, "run"));
  // Set expand property to FALSE for the buttons
  gtk_widget_set_hexpand(run_button, FALSE);
  gtk_widget_set_vexpand(run_button, FALSE);
  // Connect the buttons to their handlers / callbacks
  g_signal_connect(run_button, "clicked", G_CALLBACK(on_run_clicked), image_widget);

  // // Connect GtkEntry signals
  // GtkWidget *left_angle_entry = GTK_WIDGET(gtk_builder_get_object(builder, "left_angle_entry"));
  // g_signal_connect(left_angle_entry, "activate", G_CALLBACK(on_left_angle_entry_activate), NULL);

  // GtkWidget *right_angle_entry = GTK_WIDGET(gtk_builder_get_object(builder, "right_angle_entry"));
  // g_signal_connect(right_angle_entry, "activate", G_CALLBACK(on_right_angle_entry_activate), NULL);

  // // Set expand property to FALSE for the buttons
  // gtk_widget_set_hexpand(run_button, FALSE);
  // gtk_widget_set_vexpand(run_button, FALSE);
  GtkWidget *left_angle_entry = GTK_WIDGET(gtk_builder_get_object(builder, "left_angle_entry"));
  GtkWidget *right_angle_entry = GTK_WIDGET(gtk_builder_get_object(builder, "right_angle_entry"));
  if (!left_angle_entry || !right_angle_entry)
  {
    g_error("Failed to retrieve GtkEntry widgets.");
  }

  char *left_angle_text = g_strdup_printf("%.2f", DEFAULT_LEFT_ANGLE);
  gtk_entry_set_text(GTK_ENTRY(left_angle_entry), left_angle_text);
  g_free(left_angle_text);

  char *right_angle_text = g_strdup_printf("%.2f", DEFAULT_RIGHT_ANGLE);
  gtk_entry_set_text(GTK_ENTRY(right_angle_entry), right_angle_text);
  g_free(right_angle_text);

  g_signal_connect(left_angle_entry, "activate", G_CALLBACK(on_left_angle_entry_activate), &left_angle);    // updates when the user presses enter
  g_signal_connect(right_angle_entry, "activate", G_CALLBACK(on_right_angle_entry_activate), &right_angle); // updates when the user presses enterpresses enter
  // Connect the buttons to their handlers / callbacks

  g_signal_connect(run_button, "clicked", G_CALLBACK(on_run_clicked), image_widget);

  // Resizing directives
  // Set the expand properties for the image widget
  gtk_widget_set_hexpand(image_widget, TRUE);
  gtk_widget_set_vexpand(image_widget, TRUE);
  // Get the screen size
  GdkScreen *screen = gdk_screen_get_default();
  gint screen_width = gdk_screen_get_width(screen);
  gint screen_height = gdk_screen_get_height(screen);

  // Set the window size to half the screen size
  gtk_window_set_default_size(GTK_WINDOW(window), screen_width, screen_height);

  // Load a sample image and set it to the image widget
  pixbuf = gdk_pixbuf_new_from_file("images/Logo_OCR_WS_fanette.png", NULL);
  if (pixbuf)
  {
    process_and_display_image(image_widget, pixbuf);
    g_object_unref(pixbuf);
  }
  else
  {
    my_print("Failed to load sample image\n");
    return 1; // Exit the program if the image fails to load
  }

  // Load and apply CSS
  GtkCssProvider *css_provider = gtk_css_provider_new();
  gtk_css_provider_load_from_path(css_provider, "source/gtk/style/style.css", NULL);
  gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                            GTK_STYLE_PROVIDER(css_provider),
                                            GTK_STYLE_PROVIDER_PRIORITY_USER);
  g_object_unref(css_provider);

  // Show the main window
  gtk_widget_show_all(window);

  // Start the GTK main loop
  gtk_main();

  return 0;
}