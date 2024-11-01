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
GdkPixbuf *rotate_pixbuf(GdkPixbuf *src, double angle)
{
    int src_width = gdk_pixbuf_get_width(src);
    int src_height = gdk_pixbuf_get_height(src);
    int channels = gdk_pixbuf_get_n_channels(src);
    int rowstride = gdk_pixbuf_get_rowstride(src);
    guchar *src_pixels = gdk_pixbuf_get_pixels(src);

    // Convert angle to radians
    double radians = angle * M_PI / 180.0;
    double cos_angle = cos(radians);
    double sin_angle = sin(radians);

    // Calculate dimensions of the rotated image
    int dest_width = (int)(fabs(src_width * cos_angle) + fabs(src_height * sin_angle));
    int dest_height = (int)(fabs(src_width * sin_angle) + fabs(src_height * cos_angle));

    // Create a new pixbuf for the rotated image
    GdkPixbuf *dest = gdk_pixbuf_new(GDK_COLORSPACE_RGB, gdk_pixbuf_get_has_alpha(src), 8, dest_width, dest_height);
    guchar *dest_pixels = gdk_pixbuf_get_pixels(dest);
    int dest_rowstride = gdk_pixbuf_get_rowstride(dest);

    // Initialize the destination pixbuf to transparent (for images with alpha channel)
    memset(dest_pixels, 0, dest_height * dest_rowstride);

    // Center of the source and destination images
    int cx = src_width / 2;
    int cy = src_height / 2;
    int dcx = dest_width / 2;
    int dcy = dest_height / 2;

    // Loop over destination pixels and map them to source pixels
    for (int y = 0; y < dest_height; y++)
    {
        for (int x = 0; x < dest_width; x++)
        {
            // Calculate the corresponding source position
            int src_x = (int)((x - dcx) * cos_angle + (y - dcy) * sin_angle + cx);
            int src_y = (int)(-(x - dcx) * sin_angle + (y - dcy) * cos_angle + cy);

            // Check if the source position is within bounds
            if (src_x >= 0 && src_x < src_width && src_y >= 0 && src_y < src_height)
            {
                // Get the source pixel
                guchar *src_pixel = src_pixels + src_y * rowstride + src_x * channels;
                // Get the destination pixel
                guchar *dest_pixel = dest_pixels + y * dest_rowstride + x * channels;

                // Copy pixel data (RGB and possibly alpha)
                for (int c = 0; c < channels; c++)
                {
                    dest_pixel[c] = src_pixel[c];
                }
            }
        }
    }

    return dest;
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
