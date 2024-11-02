#include "rotation_utils.h"

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
    // Calculate the rotation angle in radians
    double radians = angle * G_PI / 180.0;

    // Get the original dimensions of the image
    int original_width = gdk_pixbuf_get_width(pixbuf);
    int original_height = gdk_pixbuf_get_height(pixbuf);

    // Calculate the dimensions of the rotated image
    double abs_cos = fabs(cos(radians));
    double abs_sin = fabs(sin(radians));
    int rotated_width = (int)(original_width * abs_cos + original_height * abs_sin);
    int rotated_height = (int)(original_width * abs_sin + original_height * abs_cos);

    // Create a new pixbuf to hold the rotated image
    GdkPixbuf *rotated_pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, rotated_width, rotated_height);
    gdk_pixbuf_fill(rotated_pixbuf, 0x00000000); // Fill with transparent color

    // Get the pixel data of the original and rotated pixbufs
    guchar *src_pixels = gdk_pixbuf_get_pixels(pixbuf);
    guchar *dst_pixels = gdk_pixbuf_get_pixels(rotated_pixbuf);
    int src_rowstride = gdk_pixbuf_get_rowstride(pixbuf);
    int dst_rowstride = gdk_pixbuf_get_rowstride(rotated_pixbuf);
    int src_n_channels = gdk_pixbuf_get_n_channels(pixbuf);
    int dst_n_channels = gdk_pixbuf_get_n_channels(rotated_pixbuf);

    // Calculate the center of the original and rotated images
    double cx = original_width / 2.0;
    double cy = original_height / 2.0;
    double ncx = rotated_width / 2.0;
    double ncy = rotated_height / 2.0;

    // Perform the rotation with bilinear interpolation
    for (int y = 0; y < rotated_height; y++)
    {
        for (int x = 0; x < rotated_width; x++)
        {
            // Calculate the source coordinates
            double src_x = (x - ncx) * cos(-radians) - (y - ncy) * sin(-radians) + cx;
            double src_y = (x - ncx) * sin(-radians) + (y - ncy) * cos(-radians) + cy;

            // Check if the source coordinates are within the bounds of the original image
            if (src_x >= 0 && src_x < original_width - 1 && src_y >= 0 && src_y < original_height - 1)
            {
                // Get the integer and fractional parts of the source coordinates
                int src_x_int = (int)src_x;
                int src_y_int = (int)src_y;
                double src_x_frac = src_x - src_x_int;
                double src_y_frac = src_y - src_y_int;

                // Perform bilinear interpolation
                for (int c = 0; c < dst_n_channels; c++)
                {
                    guchar p1 = src_pixels[src_y_int * src_rowstride + src_x_int * src_n_channels + c];
                    guchar p2 = src_pixels[src_y_int * src_rowstride + (src_x_int + 1) * src_n_channels + c];
                    guchar p3 = src_pixels[(src_y_int + 1) * src_rowstride + src_x_int * src_n_channels + c];
                    guchar p4 = src_pixels[(src_y_int + 1) * src_rowstride + (src_x_int + 1) * src_n_channels + c];

                    double value = (1 - src_x_frac) * (1 - src_y_frac) * p1 +
                                   src_x_frac * (1 - src_y_frac) * p2 +
                                   (1 - src_x_frac) * src_y_frac * p3 +
                                   src_x_frac * src_y_frac * p4;

                    dst_pixels[y * dst_rowstride + x * dst_n_channels + c] = (guchar)value;
                }
            }
        }
    }

    return rotated_pixbuf;
}
/**
 * @brief Callback function to rotate the image to the left by 5 degrees.
 * @param widget The widget that triggered the function.
 * @param data Pointer to the image widget to be updated.
 * @note We want to use a custom angle which is not in the enum of GdkPixbufRotation. Thus here we use a custom function to rotate. 31/10/24
 */
void on_rotate_left_clicked(GtkWidget *widget, gpointer data)
{
    (void)data;                                                                          // Remove unused parameter warning
    GtkWidget *image = (GtkWidget *)g_object_get_data(G_OBJECT(widget), "image-widget"); // Retrieve the image widget
    if (!GTK_IS_IMAGE(image))
    {
        printf("Failed to retrieve image widget\n");
        return;
    }
    GdkPixbuf *pixbuf = image_to_pixbuf(GTK_IMAGE(image));
    if (!pixbuf)
    {
        printf("Failed to get pixbuf from image\n");
        return;
    }
    printf("Rotating left by %f degrees\n", left_angle);
    GdkPixbuf *final_pixbuf = rotate_pixbuf(pixbuf, left_angle);
    if (final_pixbuf)
    {
        process_and_display_image(image, final_pixbuf);
        g_object_unref(final_pixbuf);
    }
    else
    {
        printf("Failed to rotate pixbuf\n");
    }
    g_object_unref(pixbuf);
}

/**
 * @brief Callback function to rotate the image to the right by the specified angle.
 * @param widget The widget that triggered the function.
 * @param data User data passed to the function.
 */
void on_rotate_right_clicked(GtkWidget *widget, gpointer data)
{
    (void)data;                                                                          // Remove unused parameter warning
    GtkWidget *image = (GtkWidget *)g_object_get_data(G_OBJECT(widget), "image-widget"); // Retrieve the image widget
    if (!GTK_IS_IMAGE(image))
    {
        printf("Failed to retrieve image widget\n");
        return;
    }
    GdkPixbuf *pixbuf = image_to_pixbuf(GTK_IMAGE(image));
    if (!pixbuf)
    {
        printf("Failed to get pixbuf from image\n");
        return;
    }
    printf("Rotating right by %f degrees\n", right_angle);
    GdkPixbuf *final_pixbuf = rotate_pixbuf(pixbuf, right_angle);
    if (final_pixbuf)
    {
        process_and_display_image(image, final_pixbuf);
        g_object_unref(final_pixbuf);
    }
    else
    {
        printf("Failed to rotate pixbuf\n");
    }
    g_object_unref(pixbuf);
}