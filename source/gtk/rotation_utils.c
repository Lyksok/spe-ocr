#include "rotation_utils.h"

// global variables to store the changing angle values
double left_angle = DEFAULT_LEFT_ANGLE;
double right_angle = DEFAULT_RIGHT_ANGLE;

void on_left_angle_entry_activate(GtkEntry *entry, gpointer data)
{
  if (!entry || !data)
  {
    printf("Invalid GtkEntry or data pointer.");
    return;
  }

  const char *angle_text = gtk_entry_get_text(entry);
  if (!angle_text)
  {
    printf("Failed to retrieve text from GtkEntry.");
    return;
  }

  double *angle = (double *)data;
  *angle = g_ascii_strtod(angle_text, NULL);
  left_angle = *angle;
  printf("Left angle updated to: %.2f", *angle);
}

void on_right_angle_entry_activate(GtkEntry *entry, gpointer data)
{
  if (!entry || !data)
  {
    printf("Invalid GtkEntry or data pointer.");
    return;
  }

  const char *angle_text = gtk_entry_get_text(entry);
  if (!angle_text)
  {
    printf("Failed to retrieve text from GtkEntry.");
    return;
  }

  double *angle = (double *)data;
  *angle = g_ascii_strtod(angle_text, NULL);
  right_angle = *angle;
  printf("Right angle updated to: %.2f", *angle);
}
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

void on_rotate_left_clicked(GtkWidget *widget, gpointer data)
{
  printf("🔄 Rotating image to the left by %f degrees\n", left_angle);
  (void)widget; // Remove unused parameter warning
  GdkPixbuf *pixbuf = image_to_pixbuf(GTK_IMAGE(data));
  int width = gdk_pixbuf_get_width(pixbuf);
  int height = gdk_pixbuf_get_height(pixbuf);
  GdkPixbuf *new_pixbuf =
      gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, width, height);
  rotate_pixbuf(pixbuf, new_pixbuf, -left_angle);
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
  rotate_pixbuf(pixbuf, new_pixbuf, right_angle);
  display_pixbuf(data, new_pixbuf);
  printf("✅ Right rotation done\n");
}
/**
 * @brief Rotates a pixbuf by the specified angle using bi-linear interpolation.
 * @param src_pixbuf The original pixbuf to rotate.
 * @param dst_pixbuf The destination pixbuf to store the rotated image into.
 * @param angle The angle to rotate the image by.
 */
void rotate_pixbuf(GdkPixbuf *src, GdkPixbuf *dst, double angle)
{
  int src_width = gdk_pixbuf_get_width(src);
  int src_height = gdk_pixbuf_get_height(src);
  int dst_width = gdk_pixbuf_get_width(dst);
  int dst_height = gdk_pixbuf_get_height(dst);
  int n_channels = gdk_pixbuf_get_n_channels(src);
  int src_rowstride = gdk_pixbuf_get_rowstride(src);
  int dst_rowstride = gdk_pixbuf_get_rowstride(dst);
  guchar *src_pixels = gdk_pixbuf_get_pixels(src);
  guchar *dst_pixels = gdk_pixbuf_get_pixels(dst);

  double angle_rad = angle * G_PI / 180.0;
  double cos_angle = cos(angle_rad);
  double sin_angle = sin(angle_rad);

  for (int y = 0; y < dst_height; y++)
  {
    for (int x = 0; x < dst_width; x++)
    {
      double src_x = (x - dst_width / 2.0) * cos_angle + (y - dst_height / 2.0) * sin_angle + src_width / 2.0;
      double src_y = -(x - dst_width / 2.0) * sin_angle + (y - dst_height / 2.0) * cos_angle + src_height / 2.0;

      int x1 = (int)floor(src_x);
      int y1 = (int)floor(src_y);
      int x2 = x1 + 1;
      int y2 = y1 + 1;

      double dx = src_x - x1;
      double dy = src_y - y1;

      if (x1 >= 0 && x1 < src_width && y1 >= 0 && y1 < src_height &&
          x2 >= 0 && x2 < src_width && y2 >= 0 && y2 < src_height)
      {
        for (int c = 0; c < n_channels; c++)
        {
          guchar *p1 = src_pixels + y1 * src_rowstride + x1 * n_channels + c;
          guchar *p2 = src_pixels + y1 * src_rowstride + x2 * n_channels + c;
          guchar *p3 = src_pixels + y2 * src_rowstride + x1 * n_channels + c;
          guchar *p4 = src_pixels + y2 * src_rowstride + x2 * n_channels + c;

          double value = (1 - dx) * (1 - dy) * (*p1) + dx * (1 - dy) * (*p2) +
                         (1 - dx) * dy * (*p3) + dx * dy * (*p4);

          dst_pixels[y * dst_rowstride + x * n_channels + c] = (guchar)value;
        }
      }
      else
      {
        int nearest_x = MIN(MAX(x1, 0), src_width - 1);
        int nearest_y = MIN(MAX(y1, 0), src_height - 1);

        for (int c = 0; c < n_channels; c++)
        {
          dst_pixels[y * dst_rowstride + x * n_channels + c] =
              src_pixels[nearest_y * src_rowstride + nearest_x * n_channels + c];
        }
      }
    }
  }
}

void on_auto_rotate_clicked(GtkWidget *widget, gpointer data)
{
  (void)data;
  my_print("🔄 Auto-rotating image\n");
  (void)widget; // Remove unused parameter warning

  // // Convert the image to SDL_Surface
  // GdkPixbuf *pixbuf = image_to_pixbuf(GTK_IMAGE(data));
  // SDL_Surface *surface = gdk_pixbuf_to_sdl_surface(pixbuf);

  // // // Detect the rotation angle
  // // double angle = auto_detect_rotation_angle(surface, &param);

  // // Log the detected angle
  // my_print("📐 Detected rotation angle: %.2f degrees\n", angle);

  // // Rotate the image by the detected angle
  // int width = gdk_pixbuf_get_width(pixbuf);
  // int height = gdk_pixbuf_get_height(pixbuf);
  // GdkPixbuf *new_pixbuf =
  //     gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, width, height);
  // rotate_pixbuf(pixbuf, new_pixbuf, angle);
  // display_pixbuf(data, new_pixbuf);

  // // Clean up
  // SDL_FreeSurface(surface);
  my_print("✅ Auto-rotation done\n");
}