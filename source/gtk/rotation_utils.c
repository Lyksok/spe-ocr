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
    // Get the original dimensions of the pixbuf
    int width = gdk_pixbuf_get_width(pixbuf);
    int height = gdk_pixbuf_get_height(pixbuf);

    double angle_rad = angle * PI / 180.0;
    double sin_angle = fabs(sin(angle_rad));
    double cos_angle = fabs(cos(angle_rad));

    // Calculate the new dimensions of the rotated image
    int width_2 = (int)(width * cos_angle + height * sin_angle);
    int height_2 = (int)(width * sin_angle + height * cos_angle);

    // Create a new pixbuf to hold the rotated image
    GdkPixbuf *pixbuf_rotated = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, width_2, height_2);
    if (!pixbuf_rotated)
    {
        return NULL; // Handle error if pixbuf creation fails
    }

    // Get the pixel data and rowstride of the original and rotated pixbufs
    guchar *pixels = gdk_pixbuf_get_pixels(pixbuf);
    guchar *pixels_rotated = gdk_pixbuf_get_pixels(pixbuf_rotated);
    int rowstride = gdk_pixbuf_get_rowstride(pixbuf);
    int rowstride_2 = gdk_pixbuf_get_rowstride(pixbuf_rotated);
    int channels = gdk_pixbuf_get_n_channels(pixbuf);

    // Calculate the center of the original and rotated images
    int O_x = width / 2;
    int O_y = height / 2;
    int O_x2 = width_2 / 2;
    int O_y2 = height_2 / 2;

    // Initialize the rotated pixbuf with a transparent background
    memset(pixels_rotated, 0, height_2 * rowstride_2);

    // Perform the rotation using bilinear interpolation
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

                    pixels_rotated[(y2 * rowstride_2 + x2 * channels) + c] = (unsigned char)value;
                }
            }
        }
    }

    // Create a new pixbuf with the original dimensions and a transparent background
    GdkPixbuf *final_pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, width, height);
    if (!final_pixbuf)
    {
        g_object_unref(pixbuf_rotated);
        return NULL; // Handle error if pixbuf creation fails
    }

    // Calculate the offset to center the rotated image in the new pixbuf validatied
    int offset_x = (width - width_2) / 2;
    int offset_y = (height - height_2) / 2;

    // Ensure the offset is valid, it is the 'décalage" of the center
    if (offset_x < 0)
        offset_x = 0;
    if (offset_y < 0)
        offset_y = 0;

    // Copy the rotated image into the new pixbuf
    gdk_pixbuf_copy_area(pixbuf_rotated, 0, 0, width_2, height_2, final_pixbuf, offset_x, offset_y);
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
