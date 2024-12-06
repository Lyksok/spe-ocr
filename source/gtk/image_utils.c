#include "image_utils.h"

/**
 * @brief Processes and displays an image in an image widget.
 * It resizes the pixbuf to fit within the diagonal length and creates a new
 * pixbuf with alpha borders. And then displays the final image with alpha
 * borders.
 * @param image_widget The image widget to display the pixbuf in.
 * @param pixbuf The pixbuf to process and display.
 */
void process_and_display_image(GtkWidget *image_widget, GdkPixbuf *pixbuf)
{
  my_print("Entered process_and_display_image\n");

  // Create a new pixbuf with alpha borders and center the resized pixbuf
  GdkPixbuf *final_pixbuf = resize_with_borders(pixbuf);
  if (!final_pixbuf)
  {
    my_print("Failed to create pixbuf with alpha borders\n");
    return;
  }
  my_print("Final pixbuf with alpha borders created\n");

  // Display the final image with alpha borders
  gtk_image_set_from_pixbuf(GTK_IMAGE(image_widget), final_pixbuf);
  my_print("Image set in image_widget\n");
  // GTK_IMAGE(gtk_image_new_from_pixbuf(pixbuf));
  g_object_unref(final_pixbuf);
  my_print("Freed final pixbuf\n");
}
/**
 * @brief Callback function to change the image in the image widget.
 * @param widget The widget that triggered the function.
 */
void on_change_image(GtkWidget *widget, gpointer data)
{
  GtkWidget *image_widget = GTK_WIDGET(data);
  GtkWidget *dialog;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
  gint res;

  dialog = gtk_file_chooser_dialog_new(
      "Open File", GTK_WINDOW(gtk_widget_get_toplevel(widget)), action,
      "_Cancel", GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL);

  res = gtk_dialog_run(GTK_DIALOG(dialog));
  if (res == GTK_RESPONSE_ACCEPT)
  {
    char *filename;
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
    filename = gtk_file_chooser_get_filename(chooser);
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
    if (pixbuf)
    {
      process_and_display_image(image_widget, pixbuf);
      g_object_unref(pixbuf);
    }
    g_free(filename);
  }

  gtk_widget_destroy(dialog);
}
/**
 * @brief Callback function to save the image in the image widget.
 * @param widget The widget that triggered the function.
 */
void *on_save_image(GtkWidget *widget, gpointer data)
{
  GtkWidget *image_widget = GTK_WIDGET(data);
  GtkWidget *dialog;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
  gint res;

  dialog = gtk_file_chooser_dialog_new(
      "Save File", GTK_WINDOW(gtk_widget_get_toplevel(widget)), action,
      "_Cancel", GTK_RESPONSE_CANCEL, "_Save", GTK_RESPONSE_ACCEPT, NULL);

  res = gtk_dialog_run(GTK_DIALOG(dialog));
  if (res == GTK_RESPONSE_ACCEPT)
  {
    char *filename;
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
    filename = gtk_file_chooser_get_filename(chooser);
    GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(image_widget));
    if (pixbuf)
    {
      gdk_pixbuf_save(pixbuf, filename, "png", NULL, NULL);
    }
    g_free(filename);
  }

  gtk_widget_destroy(dialog);
  return NULL;
}

/**
 * @brief Displays a pixbuf in an image widget.
 * @param image_widget The image widget to display the pixbuf in (implicit cast
 * to GtkImage)
 * @param pixbuf The pixbuf to display.
 * @note The pixbuf is not freed
 */
void display_pixbuf(GtkWidget *image_widget, GdkPixbuf *pixbuf)
{
  gtk_image_set_from_pixbuf(GTK_IMAGE(image_widget), pixbuf);
}

/**
 * @brief AUXILIARY FUNCTION called by the resize_with_borders() function.
 * Resizes a pixbuf.
 * @param pixbuf The original pixbuf to resize.
 * @param new_width The new width of the pixbuf.
 * @param new_height The new height of the pixbuf.
 * @return A pointer to the new pixbuf.
 */
GdkPixbuf *resize_pixbuf(GdkPixbuf *pixbuf, int new_width, int new_height)
{
  int width = gdk_pixbuf_get_width(pixbuf);
  int height = gdk_pixbuf_get_height(pixbuf);

  double xscale = new_width / (double)width;
  double yscale = new_height / (double)height;

  GdkPixbuf *new_pixbuf =
      gdk_pixbuf_new(GDK_COLORSPACE_RGB, gdk_pixbuf_get_has_alpha(pixbuf), 8,
                     new_width, new_height);

  for (int x = 0; x < new_width; x++)
  {
    for (int y = 0; y < new_height; y++)
    {
      double oldx = x / xscale;
      double oldy = y / yscale;

      int top = (int)floor(oldy);
      int bottom = top + 1;
      int left = (int)floor(oldx);
      int right = left + 1;

      if (top < height && left < width && bottom < height && right < width)
      {
        for (int c = 0; c < gdk_pixbuf_get_n_channels(pixbuf); c++)
        {
          // Get the pixel values at the four corners
          double top_left = gdk_pixbuf_get_pixels(
              pixbuf)[top * gdk_pixbuf_get_rowstride(pixbuf) +
                      left * gdk_pixbuf_get_n_channels(pixbuf) + c];
          double top_right = gdk_pixbuf_get_pixels(
              pixbuf)[top * gdk_pixbuf_get_rowstride(pixbuf) +
                      right * gdk_pixbuf_get_n_channels(pixbuf) + c];
          double bottom_left = gdk_pixbuf_get_pixels(
              pixbuf)[bottom * gdk_pixbuf_get_rowstride(pixbuf) +
                      left * gdk_pixbuf_get_n_channels(pixbuf) + c];
          double bottom_right = gdk_pixbuf_get_pixels(
              pixbuf)[bottom * gdk_pixbuf_get_rowstride(pixbuf) +
                      right * gdk_pixbuf_get_n_channels(pixbuf) + c];

          // Interpolate between the top-left and top-right pixels
          double top_interp = top_left + (top_right - top_left) * (oldx - left);
          // Interpolate between the bottom-left and bottom-right pixels
          double bottom_interp =
              bottom_left + (bottom_right - bottom_left) * (oldx - left);
          // Interpolate between the top and bottom interpolated values
          double value =
              top_interp + (bottom_interp - top_interp) * (oldy - top);

          // Set the interpolated value in the new pixbuf
          gdk_pixbuf_get_pixels(
              new_pixbuf)[y * gdk_pixbuf_get_rowstride(new_pixbuf) +
                          x * gdk_pixbuf_get_n_channels(new_pixbuf) + c] =
              (guchar)value;
        }
      }
    }
  }

  return new_pixbuf;
}
/**
 * @brief Resizes a pixbuf to fit within the diagonal length and creates a new
 * pixbuf with alpha borders.
 * @param pixbuf The original pixbuf to resize and center.
 * @return A pointer to the new pixbuf with alpha borders.
 */
GdkPixbuf *resize_with_borders(GdkPixbuf *pixbuf)
{
  int original_width = gdk_pixbuf_get_width(pixbuf);
  int original_height = gdk_pixbuf_get_height(pixbuf);

  // Step 1: Calculate the diagonal length
  int diagonal_length = (int)ceil(sqrt(original_width * original_width +
                                       original_height * original_height));

  // Step 2: Add borders to make the image a square with side length equal to
  // the diagonal length
  GdkPixbuf *pixbuf_1 = create_pixbuf_with_alpha_borders(
      pixbuf, diagonal_length, diagonal_length);
  if (!pixbuf_1)
  {
    my_print("Failed to create pixbuf with alpha borders\n");
    return NULL;
  }

  // Step 3: Resize the squared image to fit within MAX_WIDTH and MAX_HEIGHT
  GdkPixbuf *resized_pixbuf = resize_pixbuf(pixbuf_1, MAX_WIDTH, MAX_HEIGHT);
  g_object_unref(pixbuf_1);
  if (!resized_pixbuf)
  {
    my_print("Failed to resize pixbuf\n");
    return NULL;
  }

  // Step 4: Add borders to resized to fit in MAX_WIDTH and MAX_HEIGHT
  GdkPixbuf *final_pixbuf =
      create_pixbuf_with_alpha_borders(resized_pixbuf, MAX_WIDTH, MAX_HEIGHT);
  g_object_unref(resized_pixbuf);
  if (!final_pixbuf)
  {
    my_print("Failed to create final pixbuf with alpha borders\n");
    return NULL;
  }

  return final_pixbuf;
}
/**
 * TODO : refactoring
 * @brief Creates a new pixbuf with alpha borders and centers the original
 * pixbuf within it.
 * @param pixbuf The original pixbuf to center.
 * @param width The width of the new pixbuf.
 * @param height The height of the new pixbuf.
 * @return A pointer to the new pixbuf with alpha borders.
 */
GdkPixbuf *create_pixbuf_with_alpha_borders(GdkPixbuf *pixbuf, int width,
                                            int height)
{
  int original_width = gdk_pixbuf_get_width(pixbuf);
  int original_height = gdk_pixbuf_get_height(pixbuf);

  GdkPixbuf *final_pixbuf =
      gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, width, height);
  gdk_pixbuf_fill(final_pixbuf, 0x00000000); // Fill with transparent color

  int offset_x = (width - original_width) / 2;
  int offset_y = (height - original_height) / 2;

  gdk_pixbuf_copy_area(pixbuf, 0, 0, original_width, original_height,
                       final_pixbuf, offset_x, offset_y);

  return final_pixbuf;
}