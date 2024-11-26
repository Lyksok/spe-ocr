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
      double src_x = (x - dst_width / 2) * cos_angle -
                     (y - dst_height / 2) * sin_angle + src_width / 2;
      double src_y = (x - dst_width / 2) * sin_angle +
                     (y - dst_height / 2) * cos_angle + src_height / 2;

      if (src_x >= 0 && src_x < src_width - 1 && src_y >= 0 && src_y < src_height - 1)
      {
        int x1 = (int)src_x;
        int y1 = (int)src_y;
        int x2 = x1 + 1;
        int y2 = y1 + 1;

        // Ensure x2 and y2 are within bounds
        if (x2 >= src_width)
          x2 = src_width - 1;
        if (y2 >= src_height)
          y2 = src_height - 1;

        double dx = src_x - x1;
        double dy = src_y - y1;

        for (int c = 0; c < n_channels; c++)
        {
          // Top-left pixel
          guchar *p1 = gdk_pixbuf_get_pixels(src_pixbuf) +
                       y1 * gdk_pixbuf_get_rowstride(src_pixbuf) +
                       x1 * n_channels + c;
          // Top-right pixel
          guchar *p2 = gdk_pixbuf_get_pixels(src_pixbuf) +
                       y1 * gdk_pixbuf_get_rowstride(src_pixbuf) +
                       x2 * n_channels + c;
          // Bottom-left pixel
          guchar *p3 = gdk_pixbuf_get_pixels(src_pixbuf) +
                       y2 * gdk_pixbuf_get_rowstride(src_pixbuf) +
                       x1 * n_channels + c;
          // Bottom-right pixel
          guchar *p4 = gdk_pixbuf_get_pixels(src_pixbuf) +
                       y2 * gdk_pixbuf_get_rowstride(src_pixbuf) +
                       x2 * n_channels + c;

          double value = (1 - dx) * (1 - dy) * (*p1) +
                         dx * (1 - dy) * (*p2) +
                         (1 - dx) * dy * (*p3) +
                         dx * dy * (*p4);

          guchar *dst_pixel = gdk_pixbuf_get_pixels(dst_pixbuf) +
                              y * gdk_pixbuf_get_rowstride(dst_pixbuf) +
                              x * n_channels + c;
          *dst_pixel = (guchar)value;
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
