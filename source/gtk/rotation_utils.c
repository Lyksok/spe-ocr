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

    // Create a transformation matrix
    cairo_matrix_t matrix;
    cairo_matrix_init_identity(&matrix);
    cairo_matrix_translate(&matrix, original_width / 2.0, original_height / 2.0);
    cairo_matrix_rotate(&matrix, radians);
    cairo_matrix_translate(&matrix, -original_width / 2.0, -original_height / 2.0);

    // Create a new pixbuf to hold the rotated image
    GdkPixbuf *rotated_pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, original_width, original_height);
    gdk_pixbuf_fill(rotated_pixbuf, 0x00000000); // Fill with transparent color

    // Create a Cairo context to perform the rotation
    cairo_surface_t *surface = cairo_image_surface_create_for_data(
        gdk_pixbuf_get_pixels(rotated_pixbuf),
        CAIRO_FORMAT_ARGB32,
        original_width,
        original_height,
        gdk_pixbuf_get_rowstride(rotated_pixbuf));
    cairo_t *cr = cairo_create(surface);

    // Apply the transformation matrix
    cairo_set_matrix(cr, &matrix);

    // Draw the original image onto the rotated pixbuf
    gdk_cairo_set_source_pixbuf(cr, pixbuf, 0, 0);
    cairo_paint(cr);

    // Free mem !
    cairo_destroy(cr);
    cairo_surface_destroy(surface);

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
    GdkPixbuf *pixbuf = image_to_pixbuf(GTK_IMAGE(image));
    if (!pixbuf)
    {
        return;
    }
    GdkPixbuf *final_pixbuf = rotate_pixbuf(pixbuf, left_angle);
    if (final_pixbuf)
    {
        process_and_display_image(widget, final_pixbuf);
        g_object_unref(final_pixbuf);
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
    GdkPixbuf *pixbuf = image_to_pixbuf(GTK_IMAGE(image));
    if (!pixbuf)
    {
        return;
    }
    GdkPixbuf *final_pixbuf = rotate_pixbuf(pixbuf, right_angle);
    if (final_pixbuf)
    {
        process_and_display_image(widget, final_pixbuf);
        g_object_unref(final_pixbuf);
    }
    g_object_unref(pixbuf);
}