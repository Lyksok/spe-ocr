#include <gtk/gtk.h>
#include <stdio.h>

// TODO refactoring of function names !
// Callback functions begin with "on_"
// All function names are in snake_case
// reminder : GTK_WIDGET is a macro that casts the pointer to a GtkWidget, just conventional

#pragma region "Image Management"

/**
 * @brief Structure to represent a pixel in an image.
 * @return Pixbuf from image provided
 */

GdkPixbuf *image_to_pixbuf(GtkImage *image)
{
  GdkPixbuf *pixbuf = NULL;
  GdkPixbuf *original_pixbuf = gtk_image_get_pixbuf(image);

  if (original_pixbuf != NULL)
  {
    int width = gdk_pixbuf_get_width(original_pixbuf);
    int height = gdk_pixbuf_get_height(original_pixbuf);
    int n_channels = gdk_pixbuf_get_n_channels(original_pixbuf);
    int rowstride = gdk_pixbuf_get_rowstride(original_pixbuf);
    guchar *pixels = gdk_pixbuf_get_pixels(original_pixbuf);

    pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, width, height);
    if (pixbuf == NULL)
    {
      g_warning("New pixbuf creation failed");
      return NULL;
    }
    guchar *new_pixels = gdk_pixbuf_get_pixels(pixbuf);

    for (int y = 0; y < height; y++)
    {
      for (int x = 0; x < width; x++)
      {
        // Copy all of the pixel data from the original pixbuf to the new pixbuf
        guchar *src_pixel = pixels + y * rowstride + x * n_channels;
        guchar *dest_pixel = new_pixels + y * gdk_pixbuf_get_rowstride(pixbuf) + x * 4;

        dest_pixel[0] = src_pixel[0];                           // Red
        dest_pixel[1] = src_pixel[1];                           // Green
        dest_pixel[2] = src_pixel[2];                           // Blue
        dest_pixel[3] = (n_channels == 4) ? src_pixel[3] : 255; // Alpha is opaque if 255
      }
    }
  }
  else // If the original pixbuf is NULL, create a new empty pixbuf
  {
    pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, 1, 1);
    if (pixbuf == NULL)
    {
      g_warning("New pixbuf creation failed");
      return NULL;
    }
    guchar *new_pixels = gdk_pixbuf_get_pixels(pixbuf);
    new_pixels[0] = 0;   // Red
    new_pixels[1] = 0;   // Green
    new_pixels[2] = 0;   // Blue
    new_pixels[3] = 255; // Alpha
  }
  return pixbuf;
}

char *open_file_dialog(GtkWidget *widget, gpointer data)
{
  GtkWidget *dialog;
  GtkFileChooser *chooser;
  GtkWidget *window = GTK_WIDGET(data);
  char *filename = NULL;

  // Create a file chooser dialog
  dialog = gtk_file_chooser_dialog_new("Open Image File",
                                       GTK_WINDOW(window),
                                       GTK_FILE_CHOOSER_ACTION_OPEN,
                                       "_Cancel", GTK_RESPONSE_CANCEL,
                                       "_Open", GTK_RESPONSE_ACCEPT,
                                       NULL);

  // Treat the user's response to the file chooser dialog
  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
  {
    chooser = GTK_FILE_CHOOSER(dialog);
    filename = gtk_file_chooser_get_filename(chooser); // Get the selected file ABSOLUTE path
  }

  gtk_widget_destroy(dialog);
  return filename;
}
GtkImage *save_file_dialog(GtkWidget *widget, gpointer data)
{
  // GtkWidget *dialog;
  // GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE; // "action" type performed by fc
  // int res;
  GtkWidget *image = NULL;
  // TODO
  return GTK_IMAGE(image);
}

/**
 * @brief Creates a GtkImage widget.
 * Initializes and returns a new GtkImage widget with sample image.
 * @param widget The triggering GtkWidget.
 * @param data Additional data for image creation.
 * @return A pointer to the new GtkImage.
 */
GtkImage *init_image_widget(GtkWidget *parent, const char *sample_image_path)
{
  GtkImage *image = GTK_IMAGE(gtk_image_new_from_file(sample_image_path));
  // gtk_widget_set_size_request(GTK_WIDGET(image), 450, 450); // Set the size of the placeholder image

  return image;
}
// Function to load an image from a file and return a GdkPixbuf
GdkPixbuf *load_pixbuf(const char *file_path)
{
  return gdk_pixbuf_new_from_file(file_path, NULL);
}

// Function to display the loaded pixbuf in the GTK application
void display_pixbuf(GtkWidget *image_widget, GdkPixbuf *pixbuf)
{
  gtk_image_set_from_pixbuf(GTK_IMAGE(image_widget), pixbuf);
}

// Function to save the displayed pixbuf to a file
void save_pixbuf(GdkPixbuf *pixbuf, const char *file_path)
{
  gdk_pixbuf_save(pixbuf, file_path, "png", NULL, NULL);
}
// Function to resize a GdkPixbuf to fit within the specified dimensions
GdkPixbuf *resize_pixbuf(GdkPixbuf *pixbuf, int width, int height)
{ // TODOOOOOO FIX !
  int original_width = gdk_pixbuf_get_width(pixbuf);
  int original_height = gdk_pixbuf_get_height(pixbuf);

  // Calculate the scaling factor to maintain the aspect ratio
  double scale = MIN((double)width / original_width, (double)height / original_height);

  int new_width = (int)(original_width * scale);
  int new_height = (int)(original_height * scale);

  return gdk_pixbuf_scale_simple(pixbuf, new_width, new_height, GDK_INTERP_BILINEAR);
}
/**
 * @brief Callback function to change the current image with a new image and pass it to image_to_pixbuf
 * @param widget The widget that triggered the function
 * @param data Pointer to the image widget to be updated
 */
void on_change_image(GtkWidget *widget, gpointer data)
{
  GtkWidget *image_widget = GTK_WIDGET(data);
  GtkWidget *dialog;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
  gint res;

  dialog = gtk_file_chooser_dialog_new("Open File",
                                       GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                       action,
                                       "_Cancel", GTK_RESPONSE_CANCEL,
                                       "_Open", GTK_RESPONSE_ACCEPT,
                                       NULL);

  res = gtk_dialog_run(GTK_DIALOG(dialog));
  if (res == GTK_RESPONSE_ACCEPT)
  {
    char *filename;
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
    filename = gtk_file_chooser_get_filename(chooser);
    GdkPixbuf *pixbuf = load_pixbuf(filename);
    // Resize the pixbuf to fit within the layout dimensions (e.g., 800x600)
    GdkPixbuf *resized_pixbuf = resize_pixbuf(pixbuf, 800, 600);
    display_pixbuf(image_widget, resized_pixbuf);

    display_pixbuf(image_widget, pixbuf);
    g_object_unref(pixbuf);
    g_free(filename);
  }

  gtk_widget_destroy(dialog);
}

#pragma endregion "Image Management"
#pragma region "GUI Management"
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
  return button;
}
/**
 * @brief Creates the menu bar for the given GTK window.
 * This function initializes menu bar for the specified
 * GTK window. It adds the menu items to load and save image from a file explorer.
 * @param window A pointer to the GTK window where the menu bar will be added.
 */
GtkWidget *init_menu_bar(GtkWidget *window, GtkWidget *image_widget)
{
  GtkWidget *menu_bar, *file_menu, *file_menu_item, *load_menu_item, *save_menu_item;

  // Creates a menu bar
  menu_bar = gtk_menu_bar_new();

  // Creates a file menu => menu bar
  file_menu = gtk_menu_new();
  file_menu_item = gtk_menu_item_new_with_label("File");
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_menu_item), file_menu);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file_menu_item);

  // Creates load menu item => file_menu
  load_menu_item = gtk_menu_item_new_with_label("Load Image");
  g_signal_connect(load_menu_item, "activate", G_CALLBACK(on_change_image), image_widget);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), load_menu_item);

  // Creates save menu item => file_menu
  save_menu_item = gtk_menu_item_new_with_label("Save Image");
  g_signal_connect(save_menu_item, "activate", G_CALLBACK(save_file_dialog), window);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), save_menu_item);

  return menu_bar;
}
#pragma endregion "GUI Management"
#pragma region "App activation"
/**
 * @brief Activates the app. Precisely, it lays out the widgets and connects the signals.
 * @param app A pointer to the GtkApplication instance.
 */
static void activate(GtkApplication *app)
{
  GtkWidget *window, *image, *button, *menu_bar, *grid, *vbox_buttons;

  // Create the main application window
  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "OCR App for Crosswords");
  gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
  gtk_window_set_resizable(GTK_WINDOW(window), TRUE);

  // Create a grid container
  grid = gtk_grid_new();
  gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
  gtk_container_add(GTK_CONTAINER(window), grid);

  // Initialize the image widget with a sample image
  image = gtk_image_new_from_file("images/abstract_background.jpg");

  // Initialize the menu bar
  menu_bar = init_menu_bar(window, image);
  gtk_grid_attach(GTK_GRID(grid), menu_bar, 0, 0, 2, 1);

  // Center the image in the grid
  gtk_widget_set_halign(image, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(image, GTK_ALIGN_CENTER);
  gtk_grid_attach(GTK_GRID(grid), image, 0, 1, 1, 1);

  // Create a vertical box for buttons
  vbox_buttons = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_grid_attach(GTK_GRID(grid), vbox_buttons, 1, 1, 1, 1);

  // Add buttons to the vertical box
  const char *button_labels[] = {"Grayscale", "Binarize", "Rotate"};
  for (int i = 0; i < sizeof(button_labels) / sizeof(button_labels[0]); i++)
  {
    button = init_button(button_labels[i], NULL, NULL);
    gtk_box_pack_start(GTK_BOX(vbox_buttons), button, FALSE, FALSE, 0);
  }

  // Proportional parameters to resize the widgets if the window is resized
  gtk_widget_set_hexpand(image, TRUE);
  gtk_widget_set_vexpand(image, TRUE);
  gtk_widget_set_hexpand(vbox_buttons, TRUE);
  gtk_widget_set_vexpand(vbox_buttons, TRUE);

  // Show all widgets
  gtk_widget_show_all(window);
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
#pragma endregion "App activation"