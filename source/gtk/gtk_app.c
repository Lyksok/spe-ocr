#include "gtk_app.h"

/**
 * @brief Converts a GtkImage to a GdkPixbuf.
 * Copies pixel data from a GtkImage to a new GdkPixbuf.
 * If the original pixbuf is NULL, uses the sample image.
 * @param image Pointer to the GtkImage.
 * @return New GdkPixbuf or NULL if creation failed.
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
    guchar *new_pixels = gdk_pixbuf_get_pixels(pixbuf); // guchar ~= unsigned char
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
  else // If the original pixbuf is NULL, use sample image
  {
    pixbuf = gdk_pixbuf_new_from_file(SAMPLE_IMAGE_PATH, NULL);

    GtkImage *sample_image = GTK_IMAGE(gtk_image_new_from_pixbuf(pixbuf));
    if (sample_image == NULL)
    {
      g_warning("Failed to load sample image from path: %s", SAMPLE_IMAGE_PATH);
      return NULL;
    }
  }
  return pixbuf;
}
/**
 * @brief Loads a pixbuf from a file.
 * @param file_path The path to the file.
 * @return A pointer to the loaded pixbuf.
 * @note The pixbuf is not freed
 */
GdkPixbuf *load_pixbuf(const char *file_path)
{
  return gdk_pixbuf_new_from_file(file_path, NULL);
}

/**
 * @brief Opens a file saving dialog and returns a GtkImage.
 *
 * @param widget The widget that triggers the dialog.
 * @param data User data passed to the function.
 * @return GtkImage* The image selected in the dialog.
 * @note Only the filename is freed // TODO rework
 */
void *on_save_image(GtkWidget *widget, gpointer data)
{
  /**
   * TODO add gtk_file_chooser_get_create_folders, gets whether file choser will offer to create new folders. See gtk_file_chooser_set_create_folders().

since: 2.18 */
  GtkWidget *dialog;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
  gint res;
  // TODO understand gtk_widget_get_toplevel
  dialog = gtk_file_chooser_dialog_new("Save File",
                                       GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                       action,
                                       "_Cancel", GTK_RESPONSE_CANCEL,
                                       "_Save", GTK_RESPONSE_ACCEPT,
                                       NULL);
  res = gtk_dialog_run(GTK_DIALOG(dialog));
  if (res == GTK_RESPONSE_CANCEL)
  {
    gtk_widget_destroy(dialog);
    return NULL;
  }
  if (res == GTK_RESPONSE_ACCEPT)
  {
    char *filename;
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
    filename = gtk_file_chooser_get_filename(chooser);
    // Save the image
    GdkPixbuf *pixbuf = image_to_pixbuf(GTK_IMAGE(data));
    if (pixbuf != NULL)
    {
      /**TODO there must be an elegant way
      Ensure the filename has a .png extension*/
      const char *extension = ".png";
      char *filename_with_extension = g_strconcat(filename, extension, NULL);
      gdk_pixbuf_save(pixbuf, filename_with_extension, "png", NULL, NULL);
      g_free(filename_with_extension);
    }
    gtk_widget_destroy(dialog);
    g_free(filename);
  }
  return NULL;
}

/**
 * LEGACY CODE
 * @brief Creates a GtkImage widget.
 * Initializes and returns a new GtkImage widget with sample image.
 * @param widget The triggering GtkWidget.
 * @param data Additional data for image creation.
 * @return A pointer to the new GtkImage.
 * @note Nothing is freed

GtkImage *init_image_widget(GtkWidget *parent, const char *sample_image_path)
{
  GtkImage *image = GTK_IMAGE(gtk_image_new_from_file(sample_image_path));

  return image;
}
*/

/**
 * @brief Displays a pixbuf in an image widget.
 * @param image_widget The image widget to display the pixbuf in (implicit cast to GtkImage)
 * @param pixbuf The pixbuf to display.
 * @note The pixbuf is not freed
 */
void display_pixbuf(GtkWidget *image_widget, GdkPixbuf *pixbuf)
{
  gtk_image_set_from_pixbuf(GTK_IMAGE(image_widget), pixbuf);
}

/**
 * @brief Resizes a pixbuf to fit within the specified dimensions.
 * @param pixbuf The pixbuf to resize.
 * @param width The maximum width of the resized pixbuf.
 * @param height The maximum height of the resized pixbuf.
 * @return A pointer to the resized pixbuf with the adequate aspect ratio and borders.
 * @note The pixbuf is not freed
 */
GdkPixbuf *resize_pixbuf(GdkPixbuf *pixbuf, int width, int height)
{ // TODOOOOOO FIX !
  int original_width = gdk_pixbuf_get_width(pixbuf);
  int original_height = gdk_pixbuf_get_height(pixbuf);

  // Calculate the scaling factor to maintain the aspect ratio
  double scale = MIN((double)width / original_width, (double)height / original_height);

  int new_width = (int)(original_width * scale);
  int new_height = (int)(original_height * scale);

  return gdk_pixbuf_scale_simple(pixbuf, new_width, new_height, GDK_INTERP_BILINEAR); // Bilinear interpolation to resize the image
}

/**
 * @brief Creates a alpha-background pixbuf of the specified dimensions.
 * @param width The width of the pixbuf.
 * @param height The height of the pixbuf.
 * @return A pointer to the new white pixbuf.
 * @note The pixbuf is not freed. You should call it with the sizes as macros for consistency.
 */
GdkPixbuf *create_alpha_pixbuf(int width, int height)
{
  GdkPixbuf *pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, width, height);
  guchar *pixels = gdk_pixbuf_get_pixels(pixbuf);
  int rowstride = gdk_pixbuf_get_rowstride(pixbuf);

  for (int y = 0; y < height; y++)
  {
    for (int x = 0; x < width; x++)
    {
      guchar *pixel = pixels + y * rowstride + x * 4;
      pixel[0] = 0; // Red
      pixel[1] = 0; // Green
      pixel[2] = 0; // Blue
      pixel[3] = 0; // Alpha
    }
  }

  return pixbuf;
}

// Function to add borders to a pixbuf to fit within the specified dimensions
GdkPixbuf *calculate_borders(GdkPixbuf *pixbuf, int width, int height)
{
  int original_width = gdk_pixbuf_get_width(pixbuf);
  int original_height = gdk_pixbuf_get_height(pixbuf);

  GdkPixbuf *white_pixbuf = create_alpha_pixbuf(width, height);
  int offset_x = (width - original_width) / 2;
  int offset_y = (height - original_height) / 2;

  gdk_pixbuf_copy_area(pixbuf, 0, 0, original_width, original_height, white_pixbuf, offset_x, offset_y);

  return white_pixbuf;
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
  if (res == GTK_RESPONSE_CANCEL)
  {
    gtk_widget_destroy(dialog);
    return;
  }
  if (res == GTK_RESPONSE_ACCEPT)
  {
    char *filename;
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
    filename = gtk_file_chooser_get_filename(chooser);

    // Load the selected image
    GdkPixbuf *loaded_pixbuf = load_pixbuf(filename);
    g_free(filename);

    // Resize the image if necessary

    int width = gdk_pixbuf_get_width(loaded_pixbuf);
    int height = gdk_pixbuf_get_height(loaded_pixbuf);
    GdkPixbuf *resized_pixbuf = resize_pixbuf(loaded_pixbuf, width, height);

    // Apply white borders to the resized image
    GdkPixbuf *final_pixbuf = calculate_borders(resized_pixbuf, IMAGE_WIDTH, IMAGE_HEIGHT);

    // Display the final image with white borders
    gtk_image_set_from_pixbuf(GTK_IMAGE(image_widget), final_pixbuf);

    // Free the pixbufs
    g_object_unref(loaded_pixbuf);
    g_object_unref(resized_pixbuf);
    g_object_unref(final_pixbuf);
  }

  gtk_widget_destroy(dialog);
}

/**
 * @brief EXPLICIT CONVERSION METHOD! => Converts a GdkPixbuf to an SDL_Surface.
 * GdkPixbuf is used in GTK for image handling whereas SDL_Surface is used in SDL for rendering surfaces. Necessary to call preprocessing functions in the app.
 * @param surface The surface to convert to a GdkPixbuf.
 * @return A pointer to the new GdkPixbuf.
 */
GdkPixbuf *sdl_surface_to_gdk_pixbuf(SDL_Surface *surface)
{
  // Retrieve the surface data about its pixels
  int width = surface->w;
  int height = surface->h;
  int rowstride = surface->pitch;
  guchar *pixels = (guchar *)surface->pixels;

  // Call gdk_pixbuf_new_from_data to create a new GdkPixbuf from the data retrieved
  GdkPixbuf *pixbuf = gdk_pixbuf_new_from_data(
      pixels,                                // data = pixels
      GDK_COLORSPACE_RGB,                    // colorspace = RGB
      surface->format->Amask ? TRUE : FALSE, // has_alpha ? Yes if Amask != 0
      8,                                     // Currently only RGB images with 8 bits per sample are supported
      width,
      height,
      rowstride, // nb bytes of a row's size
      NULL,      // no supported destroy
      NULL);     // no supported destroy

  return pixbuf;
}
/**
 * @brief EXPLICIT CONVERSION METHOD! => Converts an SDL_Surface to a GdkPixbuf.
 * GdkPixbuf is used in GTK for image handling whereas SDL_Surface is used in SDL for rendering surfaces. Necessary to call preprocessing functions in the app.
 * @param pixbuf The pixbuf to convert to an SDL_Surface.
 * @return A pointer to the new SDL_Surface.
 */
SDL_Surface *gdk_pixbuf_to_sdl_surface(GdkPixbuf *pixbuf)
{
  // Retrieve the pixbuf data about its pixels
  int width = gdk_pixbuf_get_width(pixbuf);
  int height = gdk_pixbuf_get_height(pixbuf);
  int rowstride = gdk_pixbuf_get_rowstride(pixbuf);
  int n_channels = gdk_pixbuf_get_n_channels(pixbuf);
  guchar *pixels = gdk_pixbuf_get_pixels(pixbuf);

  // Call SDL_CreateRGBSurfaceFrom to create a new SDL_Surface from the data retrieved
  SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(
      pixels,
      width,
      height,
      8 * n_channels,                    // 8 bits per pixel * number of channels
      rowstride,                         // nb bytes of a row's size
      0x000000FF,                        // Red mask as hexa
      0x0000FF00,                        // Green mask as hexa
      0x00FF0000,                        // Blue mask as hexa
      n_channels == 4 ? 0xFF000000 : 0); // Alpha mask if 4  (as hexa) channels (= RGBA) else 0 (= RGB)

  return surface;
}
/**
 * @brief Callback function to convert the image to grayscaled.
 *  @param widget The widget that triggered the function
 * @param data Pointer to the image widget to be updated
 * @note The surface is freed but the pixbuf is NOT
 */
void on_grayscale_clicked(GtkWidget *widget, gpointer data)
{
  (void)widget; // Remove unused parameter warning
  GdkPixbuf *pixbuf = image_to_pixbuf(GTK_IMAGE(data));
  SDL_Surface *surface = gdk_pixbuf_to_sdl_surface(pixbuf);
  convert_to_grayscale(surface);
  GdkPixbuf *new_pixbuf = sdl_surface_to_gdk_pixbuf(surface);
  display_pixbuf(data, new_pixbuf);

  SDL_FreeSurface(surface); // Free the surface
}
/**
 * @brief Callback function to convert the image to binarized.
 *  @param widget The widget that triggered the function
 * @param data Pointer to the image widget to be updated
 * @note The surface is freed but the pixbuf is NOT
 */
void on_binarize_clicked(GtkWidget *widget, gpointer data)
{
  (void)widget; // Remove unused parameter warning
  GdkPixbuf *pixbuf = image_to_pixbuf(GTK_IMAGE(data));
  SDL_Surface *surface = gdk_pixbuf_to_sdl_surface(pixbuf);
  convert_to_binarized_global(surface);
  GdkPixbuf *new_pixbuf = sdl_surface_to_gdk_pixbuf(surface);
  display_pixbuf(data, new_pixbuf);

  SDL_FreeSurface(surface); // Free the surface
}

// global variables to store the changing angle values
double left_angle = DEFAULT_LEFT_ANGLE;
double right_angle = DEFAULT_RIGHT_ANGLE;

/**
 * @brief Callback function to handle the "activate" signal of the GtkEntry widgets. This is a lot of code for a simple function!
 * @param entry The GtkEntry widget.
 * @param data Pointer to the rotation angle macro to update.
 */
void on_angle_entry_activate(GtkEntry *entry, gpointer data)
{
  double *angle = (double *)data;
  const char *angle_text = gtk_entry_get_text(entry);
  *angle = g_ascii_strtod(angle_text, NULL); // convert string to double
}
/**
 * @brief Rotates a GdkPixbuf by a specified number of degrees using the macros of height and width allocated to the pixbuf.
 * @param pixbuf The pixbuf to rotate.
 * @param angle The angle in degrees to rotate the pixbuf.
 * @return A new GdkPixbuf that is the rotated version of the input pixbuf.
 */
GdkPixbuf *rotate_pixbuf(GdkPixbuf *pixbuf, double angle)
{
  // just in case the values changes from the initial macros
  int width = gdk_pixbuf_get_width(pixbuf);
  int height = gdk_pixbuf_get_height(pixbuf);

  double angle_rad = angle * PI / 180;
  // Calculate dimensions and angles of the soon-to-be rotated image
  double sin_angle = fabs(sin(angle_rad));
  double cos_angle = fabs(cos(angle_rad));

  /**
   * Pytagorean theorem to calculate the new size of the image
   * Goal is to keep the image in the same dimensions after n rotations
   */

  int width_2 = (int)(IMAGE_WIDTH * cos_angle + IMAGE_HEIGHT * sin_angle);
  int height_2 = (int)(IMAGE_WIDTH * sin_angle + IMAGE_HEIGHT * cos_angle);

  // Create rotated pixbuf (copied process from to sdl_surface_to_gdk_pixbuf)
  GdkPixbuf *pixbuf_rotated = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, width_2, height_2);

  // These are documented in the sdl_surface_to_gdk_pixbuf function if needed
  guchar *pixels = gdk_pixbuf_get_pixels(pixbuf);
  guchar *pixels_rotated = gdk_pixbuf_get_pixels(pixbuf_rotated);

  // Constrain the rowstride to keep the dimensions
  int rowstride = gdk_pixbuf_get_rowstride(pixbuf); // size of a row of pixels
  int rowstride_2 = gdk_pixbuf_get_rowstride(pixbuf_rotated);

  int channels = gdk_pixbuf_get_n_channels(pixbuf); // RGB or RGBA
  // Calculate the center of the image bcs rotation happens on the center
  // /!\ width_2 = height_2 for a square image
  int O_x = width / 2;
  int O_y = height / 2;
  int O_x2 = width_2 / 2;
  int O_y2 = height_2 / 2;

  // Init dest pixbuf ie the rotated pixbuf
  memset(pixels_rotated, 0, height_2 * rowstride_2);
  /** Process
   * Rotation formula explanation at : https://homepages.inf.ed.ac.uk/rbf/HIPR2/rotate.htm
   *
   * ************ BILINEAR INTERPOLATION ************
   * This technique is used to resize the image after n rotations.
   * The B. I. of a pixel combines of the values of the four nearest pixels, weighted by their distances to the pixel.
   * Visual representation : https://en.wikipedia.org/wiki/Bilinear_interpolation#/media/File:Bilinear_interpolation_visualisation.svg
   * The process is as follows :
   * 1. Find coordinates of the 4 nearest pixels in the original image.
   * 2. Calculate the fractional parts of x and y.
   * 3. Calculate the weights of the 4 pixels.
   * 4. Multiply the pixel values by the weights.
   * 5. Sum the results.
   * ************************************************
   */
  for (int x2 = 0; x2 < width_2; x2++)
  {
    for (int y2 = 0; y2 < height_2; y2++)
    {
      // Calculate the corresponding source coordinates
      double x = (x2 - O_x2) * cos(angle_rad) + (y2 - O_y2) * sin(angle_rad) + O_x;
      double y = -(x2 - O_x2) * sin(angle_rad) + (y2 - O_y2) * cos(angle_rad) + O_y;

      if (x >= 0 && x < width - 1 && y >= 0 && y < height - 1)
      {
        int x1 = (int)x;
        int y1 = (int)y;
        double dx = x - x1;
        double dy = y - y1;

        for (int c = 0; c < channels; c++)
        {
          double value = (1 - dx) * (1 - dy) * pixels[(y1 * rowstride + x1 * channels) + c] +
                         dx * (1 - dy) * pixels[(y1 * rowstride + (x1 + 1) * channels) + c] +
                         (1 - dx) * dy * pixels[((y1 + 1) * rowstride + x1 * channels) + c] +
                         dx * dy * pixels[((y1 + 1) * rowstride + (x1 + 1) * channels) + c];

          pixels_rotated[(y2 * rowstride_2 + x2 * channels) + c] = (unsigned char)value; // eq. to guchar
        }
      }
    }
  }

  // Create a new pixbuf with the original dimensions and a transparent background
  GdkPixbuf *final_pixbuf = create_alpha_pixbuf(width, height);

  // Calculate the offset to center the rotated image in the new pixbuf
  int offset_x = (width - width_2) / 2;
  int offset_y = (height - height_2) / 2;

  // Copy the rotated image into the new pixbuf
  gdk_pixbuf_copy_area(pixbuf_rotated, 0, 0, width_2, height_2, final_pixbuf, offset_x, offset_y); // copy the pixels occupied by the rotated pixbuf onto the final pixbuf
  g_object_unref(pixbuf_rotated);

  return final_pixbuf;
}

/**
 * @brief Callback function to rotate the image to the left by 5 degrees.
 * @param widget The widget that triggered the function.
 * @param data Pointer to the image widget to be updated.
 * @note We want to use a custom angle which is not in the enum of GdkPixbufRotation. Thus here we use a custom function to rotate. 31/10/24
 */
void on_rotate_left_clicked(GtkWidget *widget, gpointer data)
{
  (void)data; // Remove unused parameter warning
  GtkWidget *image = (GtkWidget *)g_object_get_data(G_OBJECT(widget), "image-widget");
  GdkPixbuf *pixbuf = image_to_pixbuf(GTK_IMAGE(image)); // Convert GtkImage to GdkPixbuf
  if (!pixbuf)
  {
    return; // Handle error if pixbuf is NULL
  }
  GdkPixbuf *final_pixbuf = rotate_pixbuf(pixbuf, left_angle); // Rotate the pixbuf by the specified angle
  if (final_pixbuf)
  {
    display_pixbuf(image, final_pixbuf); // Display the rotated pixbuf in the image widget
    g_object_unref(final_pixbuf);        // Free the rotated pixbuf
  }
  g_object_unref(pixbuf); // Free the original pixbuf
}

/**
 * @brief Callback function to rotate the image to the right by the specified angle.
 * @param widget The widget that triggered the function.
 * @param data User data passed to the function.
 */
void on_rotate_right_clicked(GtkWidget *widget, gpointer data)
{
  (void)data; // Remove unused parameter warning
  GtkWidget *image = (GtkWidget *)g_object_get_data(G_OBJECT(widget), "image-widget");
  GdkPixbuf *pixbuf = image_to_pixbuf(GTK_IMAGE(image)); // Convert GtkImage to GdkPixbuf
  if (!pixbuf)
  {
    return; // Handle error if pixbuf is NULL
  }
  GdkPixbuf *final_pixbuf = rotate_pixbuf(pixbuf, right_angle); // Rotate the pixbuf by the specified angle
  if (final_pixbuf)
  {
    display_pixbuf(image, final_pixbuf); // Display the rotated pixbuf in the image widget
    g_object_unref(final_pixbuf);        // Free the rotated pixbuf
  }
  g_object_unref(pixbuf); // Free the original pixbuf
}

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
