#include "rotation_utils.h"

// Global variables to store the changing angle values
double left_angle = DEFAULT_LEFT_ANGLE;
double right_angle = DEFAULT_RIGHT_ANGLE;

static double cubic_interpolate(double p[4], double x)
{
  return p[1] + 0.5 * x * (p[2] - p[0] + x * (2.0 * p[0] - 5.0 * p[1] + 4.0 * p[2] - p[3] + x * (3.0 * (p[1] - p[2]) + p[3] - p[0])));
}

static double bicubic_interpolate(double p[4][4], double x, double y)
{
  double arr[4];
  arr[0] = cubic_interpolate(p[0], y);
  arr[1] = cubic_interpolate(p[1], y);
  arr[2] = cubic_interpolate(p[2], y);
  arr[3] = cubic_interpolate(p[3], y);
  return cubic_interpolate(arr, x);
}

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
      double src_x = (x - dst_width / 2.0) * cos_angle - (y - dst_height / 2.0) * sin_angle + src_width / 2.0;
      double src_y = (x - dst_width / 2.0) * sin_angle + (y - dst_height / 2.0) * cos_angle + src_height / 2.0;

      if (src_x >= 1 && src_x < src_width - 2 && src_y >= 1 && src_y < src_height - 2)
      {
        for (int c = 0; c < n_channels; c++)
        {
          double p[4][4];
          for (int m = -1; m <= 2; m++)
          {
            for (int n = -1; n <= 2; n++)
            {
              guchar *src_pixel = gdk_pixbuf_get_pixels(src_pixbuf) +
                                  ((int)src_y + m) * gdk_pixbuf_get_rowstride(src_pixbuf) +
                                  ((int)src_x + n) * n_channels + c;
              p[m + 1][n + 1] = (double)*src_pixel;
            }
          }
          guchar *dst_pixel = gdk_pixbuf_get_pixels(dst_pixbuf) +
                              y * gdk_pixbuf_get_rowstride(dst_pixbuf) +
                              x * n_channels + c;
          *dst_pixel = (guchar)bicubic_interpolate(p, src_x - (int)src_x, src_y - (int)src_y);
        }
      }
    }
  }

  printf("✅ Rotation done\n");
}

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
  GdkPixbuf *new_pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, width, height);
  rotate_pixbuf(pixbuf, new_pixbuf, left_angle);
  display_pixbuf(data, new_pixbuf);
  printf("✅ Left rotation done\n");
}

void on_rotate_right_clicked(GtkWidget *widget, gpointer data)
{
  printf("🔄 Rotating image to the right by %f degrees\n", right_angle);
  (void)widget; // Remove unused parameter warning
  GdkPixbuf *pixbuf = image_to_pixbuf(GTK_IMAGE(data));
  int width = gdk_pixbuf_get_width(pixbuf);
  int height = gdk_pixbuf_get_height(pixbuf);
  GdkPixbuf *new_pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, width, height);
  rotate_pixbuf(pixbuf, new_pixbuf, -right_angle);
  display_pixbuf(data, new_pixbuf);
  printf("✅ Right rotation done\n");
}
