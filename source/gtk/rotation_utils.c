#include "rotation_utils.h"

// global variables to store the changing angle values
double left_angle = DEFAULT_LEFT_ANGLE;
double right_angle = DEFAULT_RIGHT_ANGLE;

/**
 * @brief Callback function to handle the "activate" signal of the GtkEntry
 * widgets.
 * @param entry The GtkEntry widget.
 * @param data Pointer to the rotation angle macro to update.
 */
void on_angle_entry_activate(GtkEntry *entry, gpointer data) {
  double *angle = (double *)data;
  const char *angle_text = gtk_entry_get_text(entry);
  *angle = g_ascii_strtod(angle_text, NULL); // convert string to double
}
/**
 * @brief Rotates a pixbuf by the specified angle using bi-linear interpolation.
 * @param src_pixbuf The original pixbuf to rotate.
 * @param dst_pixbuf The destination pixbuf to store the rotated image into.
 * @param angle The angle to rotate the image by.
 */
void rotate_pixbuf(GdkPixbuf *src_pixbuf, GdkPixbuf *dst_pixbuf, double angle) {
  /**
   * Process
   * Rotation formula explanation at :
   * https://homepages.inf.ed.ac.uk/rbf/HIPR2/rotate.htm
   *
   * ************ BILINEAR INTERPOLATION ************
   * This technique is used to resize the image after n rotations.
   * The B. I. of a pixel combines of the values of the four nearest pixels,
   * weighted by their distances to the pixel.
   * Visual representation :
   * https://en.wikipedia.org/wiki/Bilinear_interpolation#/media/
   *
   * The process is as follows :
   * 1. Find coordinates of the 4 nearest pixels in the original image.
   * 2. Calculate the fractional parts of x and y.
   * 3. Calculate the weights of the 4 pixels.
   * 4. Multiply the pixel values by the weights.
   * 5. Sum the results.
   * ************************************************
   */
  int width_src = gdk_pixbuf_get_width(src_pixbuf);
  int height_src = gdk_pixbuf_get_height(src_pixbuf);
  int width_dst = gdk_pixbuf_get_width(dst_pixbuf);
  int height_dst = gdk_pixbuf_get_height(dst_pixbuf);
  double radians = angle * PI / 180.0;

  int ncx = width_dst / 2;
  int ncy = height_dst / 2;
  int cx = width_src / 2;
  int cy = height_src / 2;

  gdk_pixbuf_fill(dst_pixbuf, 0x00000000); // Fill with transparent color

  for (int y = 0; y < height_dst; y++) {
    for (int x = 0; x < width_dst; x++) {
      double src_x = (x - ncx) * cos(-radians) - (y - ncy) * sin(-radians) + cx;
      double src_y = (x - ncx) * sin(-radians) + (y - ncy) * cos(-radians) + cy;

      if (src_x >= 0 && src_x < width_src - 1 && src_y >= 0 &&
          src_y < height_src - 1) {
        int src_x_int = (int)src_x;
        int src_y_int = (int)src_y;
        /** Interpolation weights: fractional parts of the original
         * coordinates x and y.
         *  ==> dx: "how far are we from the nearest pixel to the left?"
         *  ==> dy: "how far are we from the nearest pixel to the top?"
         *
         * These weights are used in bilinear interpolation to
         * determine the contribution
         * of the four nearest pixels to the final interpolated pixel
         * value.
         */
        double dx = src_x - src_x_int;
        double dy = src_y - src_y_int;

        for (int c = 0; c < gdk_pixbuf_get_n_channels(src_pixbuf); c++) {
          // Get the pixel values of the four nearest pixels
          // p1: top-left, p2: top-right, p3: bottom-left,
          // p4: bottom-right
          guchar p1 = gdk_pixbuf_get_pixels(
              src_pixbuf)[src_y_int * gdk_pixbuf_get_rowstride(src_pixbuf) +
                          src_x_int * gdk_pixbuf_get_n_channels(src_pixbuf) +
                          c];
          guchar p2 = gdk_pixbuf_get_pixels(src_pixbuf)
              [src_y_int * gdk_pixbuf_get_rowstride(src_pixbuf) +
               (src_x_int + 1) * gdk_pixbuf_get_n_channels(src_pixbuf) + c];
          guchar p3 = gdk_pixbuf_get_pixels(src_pixbuf)
              [(src_y_int + 1) * gdk_pixbuf_get_rowstride(src_pixbuf) +
               src_x_int * gdk_pixbuf_get_n_channels(src_pixbuf) + c];
          guchar p4 = gdk_pixbuf_get_pixels(src_pixbuf)
              [(src_y_int + 1) * gdk_pixbuf_get_rowstride(src_pixbuf) +
               (src_x_int + 1) * gdk_pixbuf_get_n_channels(src_pixbuf) + c];

          guchar value =
              (guchar)((1 - dx) * (1 - dy) * p1 + dx * (1 - dy) * p2 +
                       (1 - dx) * dy * p3 + dx * dy * p4);
          gdk_pixbuf_get_pixels(
              dst_pixbuf)[y * gdk_pixbuf_get_rowstride(dst_pixbuf) +
                          x * gdk_pixbuf_get_n_channels(dst_pixbuf) + c] =
              value;
        }
      }
    }
  }
}

/**
 * @brief Callback function to rotate the image to the left by the specified
 * angle.
 * @param widget The widget that triggered the function.
 * @param data User data passed to the function.
 */
void on_rotate_left_clicked(GtkWidget *widget, gpointer data) {
  (void)data; // Remove unused warning
  GtkWidget *image =
      (GtkWidget *)g_object_get_data(G_OBJECT(widget), "image-widget");
  if (!GTK_IS_IMAGE(image)) {
    printf("Failed to retrieve image widget\n");
    return;
  }
  GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(image));
  if (!pixbuf) {
    printf("Failed to get pixbuf from image\n");
    return;
  }
  printf("Rotating left by %f degrees\n", left_angle);

  int width = gdk_pixbuf_get_width(pixbuf);
  int height = gdk_pixbuf_get_height(pixbuf);
  GdkPixbuf *rotated_pixbuf = gdk_pixbuf_new(
      GDK_COLORSPACE_RGB, gdk_pixbuf_get_has_alpha(pixbuf), 8, width, height);

  rotate_pixbuf(pixbuf, rotated_pixbuf, left_angle);
  gtk_image_set_from_pixbuf(GTK_IMAGE(image), rotated_pixbuf);
  g_object_unref(rotated_pixbuf);
}
/**
 * @brief Callback function to rotate the image to the right by the specified
 * angle.
 * @param widget The widget that triggered the function.
 * @param data User data passed to the function.
 */
void on_rotate_right_clicked(GtkWidget *widget, gpointer data) {
  (void)data; // Remove unused warning
  GtkWidget *image =
      (GtkWidget *)g_object_get_data(G_OBJECT(widget), "image-widget");
  if (!GTK_IS_IMAGE(image)) {
    printf("Failed to retrieve image widget\n");
    return;
  }
  GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(image));
  if (!pixbuf) {
    printf("Failed to get pixbuf from image\n");
    return;
  }
  printf("Rotating right by %f degrees\n", right_angle);

  int width = gdk_pixbuf_get_width(pixbuf);
  int height = gdk_pixbuf_get_height(pixbuf);
  GdkPixbuf *rotated_pixbuf = gdk_pixbuf_new(
      GDK_COLORSPACE_RGB, gdk_pixbuf_get_has_alpha(pixbuf), 8, width, height);

  rotate_pixbuf(pixbuf, rotated_pixbuf, right_angle);
  gtk_image_set_from_pixbuf(GTK_IMAGE(image), rotated_pixbuf);
  g_object_unref(rotated_pixbuf);
}