#include "main.h"

/**
 * @brief Callback function to destroy a widget.
 * @param widget The widget to be destroyed.
 * @return gboolean Always returns FALSE to stop the timeout.
 */
gboolean destroy_widget(gpointer widget)
{
  /**
   * TODO : this forcing method to free is not working, thus the leaks are
   * relative to the libpango as side effect of gtk
   * 1. This loop goes through each element in the `children` GList
   * 2/ Converts the child data to a GTK_WIDGET
   * 3. Ultimatly destroys the widget
   * 01/11/24
   */
  if (GTK_IS_WIDGET(widget))
  {
    GList *children, *p;
    // p pointer to current element
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
 * @brief Creates a button widget.
 *
 * This function initializes and returns a new GtkButton widget.
 *
 * @param label The label for the button.
 * @param callback The callback function to be called when the button is
 * clicked.
 * @param data A gpointer to any additional data needed for the callback.
 * @return A pointer to the new GtkButton.
 */
GtkWidget *init_button(const char *label, GCallback callback, gpointer data)
{
  GtkWidget *button = gtk_button_new_with_label(label);
  if (callback != NULL)
    // connects the "clicked" signal to the callback function
    g_signal_connect(button, "clicked", callback, data);

  // Align the text to the left
  GtkWidget *txt_widget = gtk_bin_get_child(GTK_BIN(button));
  gtk_widget_set_halign(txt_widget, GTK_ALIGN_START);

  // Set the image widget as data on the button
  g_object_set_data(G_OBJECT(button), "image-widget", data);

  return button;
}
/**
 * @brief Creates the menu bar for the given GTK window.
 * This function initializes menu bar for the specified
 * GTK window. It adds the menu items to load and save image from a file
 * explorer.
 * @param image_widget The image widget to be updated when clicking on the
 * submenus.
 */
GtkWidget *init_menu_bar(GtkWidget *image_widget)
{
  GtkWidget *menu_bar, *file_menu, *file_menu_item, *load_menu_item,
      *save_menu_item;

  // Creates a menu bar
  menu_bar = gtk_menu_bar_new();
  // Creates a file menu => menu bar
  file_menu = gtk_menu_new();
  file_menu_item = gtk_menu_item_new_with_label("📁 File");
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_menu_item), file_menu);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file_menu_item);

  // Creates load menu item => file_menu
  load_menu_item = gtk_menu_item_new_with_label("Load Image");
  g_signal_connect(load_menu_item, "activate", G_CALLBACK(on_change_image),
                   image_widget);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), load_menu_item);

  // Creates save menu item => file_menu
  save_menu_item = gtk_menu_item_new_with_label("Save Image");
  g_signal_connect(save_menu_item, "activate", G_CALLBACK(on_save_image),
                   image_widget);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), save_menu_item);

  return menu_bar;
}

/**
 * @brief Activates the app. Precisely, it lays out the widgets and connects
 * the signals.
 * @param app A pointer to the GtkApplication instance.
 */
static void activate(GtkApplication *app, gpointer user_data)
{
  GtkWidget *window;
  GtkWidget *main_box;
  GtkWidget *menu_bar;
  GtkWidget *grid;
  GtkWidget *image_container;
  GtkWidget *image_widget;
  GtkWidget *vbox_buttons;
  GtkWidget *button;

  (void)user_data; // Unused parameter removed warning
  // Create a new window
  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window),
                       "Organized Chaotic Results software");
  gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

  // Create a main box to hold the menu bar and the grid
  main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_add(GTK_CONTAINER(window), main_box);

  // Create an image widget
  image_widget = gtk_image_new();

  // Initialize the menu bar
  menu_bar = init_menu_bar(image_widget);
  gtk_box_pack_start(GTK_BOX(main_box), menu_bar, FALSE, FALSE, 0);

  // Create a grid container for the main content
  grid = gtk_grid_new();
  gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
  gtk_box_pack_start(GTK_BOX(main_box), grid, TRUE, TRUE, 0);

  // Create an image container
  image_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_halign(image_container, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(image_container, GTK_ALIGN_CENTER);
  gtk_grid_attach(GTK_GRID(grid), image_container, 0, 0, 1, 1);

  gtk_container_add(GTK_CONTAINER(image_container), image_widget);

  // Load a sample image
  GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(SAMPLE_IMAGE_PATH, NULL);
  if (pixbuf)
  {
    process_and_display_image(image_widget, pixbuf);
    g_object_unref(pixbuf);
  }
  else
  {
    printf("Failed to load image from %s\n", SAMPLE_IMAGE_PATH);
  }

  // Create a vertical box for buttons
  vbox_buttons = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_grid_attach(GTK_GRID(grid), vbox_buttons, 1, 0, 1, 1);

  // Manage entry buttons for the rotation
  // Update angle values according to user input
  GtkWidget *left_angle_entry = gtk_entry_new();
  GtkWidget *right_angle_entry = gtk_entry_new();
  char *left_angle_text = g_strdup_printf("%.2f", DEFAULT_LEFT_ANGLE);
  gtk_entry_set_text(GTK_ENTRY(left_angle_entry), left_angle_text);
  g_free(left_angle_text);

  char *right_angle_text = g_strdup_printf("%.2f", DEFAULT_RIGHT_ANGLE);
  gtk_entry_set_text(GTK_ENTRY(right_angle_entry), right_angle_text);
  g_free(right_angle_text);

  g_signal_connect(left_angle_entry, "activate",
                   G_CALLBACK(on_angle_entry_activate), &left_angle);
  // updates when the user presses enter
  g_signal_connect(right_angle_entry, "activate",
                   G_CALLBACK(on_angle_entry_activate), &right_angle);
  // updates when the user presses enter
  gtk_box_pack_start(GTK_BOX(vbox_buttons), gtk_label_new("Left angle:"), FALSE,
                     FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox_buttons), left_angle_entry, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox_buttons), gtk_label_new("Right angle:"),
                     FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox_buttons), right_angle_entry, FALSE, FALSE, 0);

  // Add buttons to the vertical box
  const char *button_labels[] = {"Grayscale", "Binarize", "Invert colors",
                                 "Rotate left", "Rotate right"};
  for (long unsigned int i = 0;
       i < sizeof(button_labels) / sizeof(button_labels[0]); i++)
  {
    button = init_button(button_labels[i], NULL, image_widget);
    gtk_box_pack_start(GTK_BOX(vbox_buttons), button, FALSE, FALSE, 0);
    // Connect the buttons to their respective effects
    if (strcmp(button_labels[i], "Grayscale") == 0)
    {
      g_signal_connect(button, "clicked", G_CALLBACK(on_grayscale_clicked),
                       image_widget);
    }
    else if (strcmp(button_labels[i], "Binarize") == 0)
    {
      g_signal_connect(button, "clicked", G_CALLBACK(on_binarize_clicked),
                       image_widget);
    }
    else if (strcmp(button_labels[i], "Rotate left") == 0)
    {
      g_signal_connect(button, "clicked", G_CALLBACK(on_rotate_left_clicked),
                       image_widget);
    }
    else if (strcmp(button_labels[i], "Rotate right") == 0)
    {
      g_signal_connect(button, "clicked", G_CALLBACK(on_rotate_right_clicked),
                       image_widget);
    }
    else if (strcmp(button_labels[i], "Invert colors") == 0)
    {
      g_signal_connect(button, "clicked", G_CALLBACK(on_invert_colors_clicked),
                       image_widget);
    }
    /**
    else if (strcmp(button_labels[i], "Detect") == 0)
    {
      g_signal_connect(button, "clicked", G_CALLBACK(on_detect_clicked),
                       image_widget);
    }
    else if (strcmp(button_labels[i], "Segment) == 0)
    {
      g_signal_connect(button, "clicked", G_CALLBACK(on_segment_clicked),
                       image_widget);
    }
    */
  }

  // Show all widgets in the window
  gtk_widget_show_all(window);

  // Show the splash screen
  show_splash_screen(app);
}

/**
 * @brief Main function to run the GTK application.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return int Status code.
 */
int main(int argc, char **argv)
{

  GtkApplication *app;
  int status;
  /*  Window creation  */
  // gtk_init() called in gtk_application_new '
  app = gtk_application_new("org.app.OCR", G_APPLICATION_DEFAULT_FLAGS);

  /*  Signal connections */
  // "activate" signal connection
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

  /*  Run event loop  */
  status = g_application_run(G_APPLICATION(app), argc, argv);

  g_object_unref(app);

  return status;
}
