#include "main.h"

/**
 * @brief Creates a button widget.
 *
 * This function initializes and returns a new GtkButton widget.
 *
 * @param label The label for the button.
 * @param callback The callback function to be called when the button is clicked.
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

  return button;
}
/**
 * @brief Creates the menu bar for the given GTK window.
 * This function initializes menu bar for the specified
 * GTK window. It adds the menu items to load and save image from a file explorer.
 * @param image_widget The image widget to be updated when clicking on the submenus.
 */
GtkWidget *init_menu_bar(GtkWidget *image_widget)
{
  GtkWidget *menu_bar, *file_menu, *file_menu_item, *load_menu_item, *save_menu_item;

  // Creates a menu bar
  menu_bar = gtk_menu_bar_new();
  // TODO remove possible legacy code
  // Creates a file menu => menu bar
  file_menu = gtk_menu_new();
  file_menu_item = gtk_menu_item_new_with_label("📁 File");
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_menu_item), file_menu);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file_menu_item);

  // Creates load menu item => file_menu
  load_menu_item = gtk_menu_item_new_with_label("Load Image");
  g_signal_connect(load_menu_item, "activate", G_CALLBACK(on_change_image), image_widget);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), load_menu_item);

  // Creates save menu item => file_menu
  save_menu_item = gtk_menu_item_new_with_label("Save Image");
  g_signal_connect(save_menu_item, "activate", G_CALLBACK(on_save_image), image_widget);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), save_menu_item);

  return menu_bar;
}

/**
 * @brief Activates the app. Precisely, it lays out the widgets and connects the signals.
 * @param app A pointer to the GtkApplication instance.
 */
static void activate(GtkApplication *app)
{
  // Show the splash screen defined in splash_screen.c
  show_splash_screen(app);

  /* Objects declaration */
  GtkWidget *window, *image, *button, *menu_bar, *grid, *vbox_buttons;
  GdkPixbuf *pixbuf, *resized_pixbuf, *final_pixbuf;

  // Create the main application window
  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Organized Chaotic Results software");
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

  /**  Get screen size preperties  */
  GdkDisplay *display = gdk_display_get_default();
  GdkMonitor *current_monitor = gdk_display_get_primary_monitor(display);
  GdkRectangle monitor_geometry;
  gdk_monitor_get_geometry(current_monitor, &monitor_geometry);
  int screen_height = monitor_geometry.height;
  int screen_width = monitor_geometry.width;

  gtk_window_set_default_size(GTK_WINDOW(window), screen_width, screen_height);

  // Create a grid container
  grid = gtk_grid_new();
  gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
  gtk_container_add(GTK_CONTAINER(window), grid);

  // Create an alpha pixbuf of the specified size
  pixbuf = create_alpha_pixbuf(IMAGE_WIDTH, IMAGE_HEIGHT);

  // Load and resize the image
  GdkPixbuf *loaded_pixbuf = load_pixbuf(SAMPLE_IMAGE_PATH);
  resized_pixbuf = resize_pixbuf(loaded_pixbuf, IMAGE_WIDTH, IMAGE_HEIGHT);

  // Add white borders if necessary
  final_pixbuf = calculate_borders(resized_pixbuf, IMAGE_WIDTH, IMAGE_HEIGHT);
  image = gtk_image_new_from_pixbuf(final_pixbuf);

  // Initialize the menu bar
  menu_bar = init_menu_bar(image);
  gtk_grid_attach(GTK_GRID(grid), menu_bar, 0, 0, 2, 1);

  // Center the image in the grid
  gtk_widget_set_halign(image, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(image, GTK_ALIGN_CENTER);
  gtk_grid_attach(GTK_GRID(grid), image, 0, 1, 1, 1);

  // Create a vertical box for buttons
  vbox_buttons = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_grid_attach(GTK_GRID(grid), vbox_buttons, 1, 1, 1, 1);

  // Manage entry buttons for the rotation
  GtkWidget *left_angle_entry = gtk_entry_new();
  GtkWidget *right_angle_entry = gtk_entry_new();
  gtk_entry_set_text(GTK_ENTRY(left_angle_entry), g_strdup_printf("%.2f", DEFAULT_LEFT_ANGLE));
  gtk_entry_set_text(GTK_ENTRY(right_angle_entry), g_strdup_printf("%.2f", DEFAULT_RIGHT_ANGLE));     //  pass the address of the angle to update
  g_signal_connect(left_angle_entry, "activate", G_CALLBACK(on_angle_entry_activate), &left_angle);   // updates when the user presses enter
  g_signal_connect(right_angle_entry, "activate", G_CALLBACK(on_angle_entry_activate), &right_angle); // updates when the user presses enter
  gtk_box_pack_start(GTK_BOX(vbox_buttons), gtk_label_new("Left angle:"), FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox_buttons), left_angle_entry, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox_buttons), gtk_label_new("Right angle:"), FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox_buttons), right_angle_entry, FALSE, FALSE, 0);

  // Add buttons to the vertical box
  const char *button_labels[] = {"⚪ Grayscale", "🤖 Binarize", "↪️ Rotate left", "↩️ Rotate right"};
  for (long unsigned int i = 0; i < sizeof(button_labels) / sizeof(button_labels[0]); i++)
  {
    button = init_button(button_labels[i], NULL, NULL);
    gtk_box_pack_start(GTK_BOX(vbox_buttons), button, FALSE, FALSE, 0);
    // Connect the buttons to their respective effects
    if (strcmp(button_labels[i], "⚪ Grayscale") == 0)
    {
      g_signal_connect(button, "clicked", G_CALLBACK(on_grayscale_clicked), image);
    }
    else if (strcmp(button_labels[i], "🤖 Binarize") == 0)
    {
      g_signal_connect(button, "clicked", G_CALLBACK(on_binarize_clicked), image);
    }
    // TODO simplfy the callbacks for rotation
    else if (strcmp(button_labels[i], "↪️ Rotate left") == 0)
    {
      double *angle = g_new(double, 1);
      *angle = DEFAULT_LEFT_ANGLE;
      g_object_set_data(G_OBJECT(button), "image-widget", image); // it
      g_signal_connect(button, "clicked", G_CALLBACK(on_rotate_left_clicked), angle);
    }
    else if (strcmp(button_labels[i], "↩️ Rotate right") == 0)
    {
      double *angle = g_new(double, 1);
      *angle = DEFAULT_RIGHT_ANGLE;
      g_object_set_data(G_OBJECT(button), "image-widget", image);
      g_signal_connect(button, "clicked", G_CALLBACK(on_rotate_right_clicked), angle);
    }
  }

  // Proportional parameters to resize the widgets if the window is resized
  gtk_widget_set_hexpand(image, TRUE);
  gtk_widget_set_vexpand(image, TRUE);
  gtk_widget_set_hexpand(vbox_buttons, TRUE);
  gtk_widget_set_vexpand(vbox_buttons, TRUE);

  // Show all widgets
  gtk_widget_show_all(window);

  // Free the objects

  g_object_unref(pixbuf);
  g_object_unref(resized_pixbuf);
  g_object_unref(final_pixbuf);
  g_object_unref(loaded_pixbuf);
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
  app = gtk_application_new("org.gtk.OCR", G_APPLICATION_DEFAULT_FLAGS); // creates a new gtk app instance

  /*  Signal connections */
  // "activate" signal connection
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL); // connects the "activate" signal to the "activate" function

  /*  Run event loop  */
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}
