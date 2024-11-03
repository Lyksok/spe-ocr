#include "rotation_utils.h"

// Global variables to store the changing angle values
double left_angle = DEFAULT_LEFT_ANGLE;
double right_angle = DEFAULT_RIGHT_ANGLE;

/**
 * @brief Callback function to handle the "activate" signal of the GtkEntry
 * widgets.
 * @param entry The GtkEntry widget.
 * @param data Pointer to the rotation angle macro to update.
 */
void on_angle_entry_activate(GtkEntry *entry, gpointer data)
{
  printf("🔄 Activating angle entry\n");
  double *angle = (double *)data;
  const char *angle_text = gtk_entry_get_text(entry);
  *angle = g_ascii_strtod(angle_text, NULL); // Convert string to double
  printf("📐 New angle: %f\n", *angle);
}

/**
 * @brief Rotates a pixbuf by the specified angle using bi-linear interpolation.
 * @param src_pixbuf The original pixbuf to rotate.
 * @param dst_pixbuf The destination pixbuf to store the rotated image into.
 * @param angle The angle to rotate the image by.
 */
void rotate_pixbuf(GdkPixbuf *src_pixbuf, GdkPixbuf *dst_pixbuf, double angle)
{
  printf("🔄 Rotating pixbuf by angle: %f\n", angle);
  int src_width = gdk_pixbuf_get_width(src_pixbuf);
  int src_height = gdk_pixbuf_get_height(src_pixbuf);
  int dst_width = gdk_pixbuf_get_width(dst_pixbuf);
  int dst_height = gdk_pixbuf_get_height(dst_pixbuf);
  int n_channels = gdk_pixbuf_get_n_channels(src_pixbuf);

  printf("📏 Source Width: %d, Source Height: %d, Destination Width: %d, "
         "Destination Height: %d, Channels: %d\n",
         src_width, src_height, dst_width, dst_height, n_channels);

  double angle_rad = angle * G_PI / 180.0;
  double cos_angle = cos(angle_rad);
  double sin_angle = sin(angle_rad);

  for (int y = 0; y < dst_height; y++)
  {
    for (int x = 0; x < dst_width; x++)
    {
      int src_x = (int)((x - dst_width / 2) * cos_angle -
                        (y - dst_height / 2) * sin_angle + src_width / 2);
      int src_y = (int)((x - dst_width / 2) * sin_angle +
                        (y - dst_height / 2) * cos_angle + src_height / 2);

      if (src_x >= 0 && src_x < src_width && src_y >= 0 && src_y < src_height)
      {
        for (int c = 0; c < n_channels; c++)
        {
          guchar *src_pixel = gdk_pixbuf_get_pixels(src_pixbuf) +
                              src_y * gdk_pixbuf_get_rowstride(src_pixbuf) +
                              src_x * n_channels + c;
          guchar *dst_pixel = gdk_pixbuf_get_pixels(dst_pixbuf) +
                              y * gdk_pixbuf_get_rowstride(dst_pixbuf) +
                              x * n_channels + c;
          *dst_pixel = *src_pixel;
        }
      }
    }
  }

  printf("✅ Rotation done\n");
}

/**
 * @brief Callback function to rotate the image to the left.
 * @param widget The widget that triggered the function.
 * @param data Pointer to the image widget to be updated.
 */
void on_rotate_left_clicked(GtkWidget *widget, gpointer data)
{
  printf("🔄 Rotating image to the left by %f degrees\n", left_angle);
  (void)widget; // Remove unused parameter warning
  GdkPixbuf *pixbuf = image_to_pixbuf(GTK_IMAGE(data));
  int width = gdk_pixbuf_get_width(pixbuf);
  int height = gdk_pixbuf_get_height(pixbuf);
  GdkPixbuf *new_pixbuf =
      gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, width, height);
  rotate_pixbuf(pixbuf, new_pixbuf, left_angle);
  display_pixbuf(data, new_pixbuf);
  printf("✅ Left rotation done\n");
}

/**
 * @brief Callback function to rotate the image to the right.
 * @param widget The widget that triggered the function.
 * @param data Pointer to the image widget to be updated.
 */
void on_rotate_right_clicked(GtkWidget *widget, gpointer data)
{
  printf("🔄 Rotating image to the right by %f degrees\n", right_angle);
  (void)widget; // Remove unused parameter warning
  GdkPixbuf *pixbuf = image_to_pixbuf(GTK_IMAGE(data));
  int width = gdk_pixbuf_get_width(pixbuf);
  int height = gdk_pixbuf_get_height(pixbuf);
  GdkPixbuf *new_pixbuf =
      gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, width, height);
  rotate_pixbuf(pixbuf, new_pixbuf, -right_angle);
  display_pixbuf(data, new_pixbuf);
  printf("✅ Right rotation done\n");
}