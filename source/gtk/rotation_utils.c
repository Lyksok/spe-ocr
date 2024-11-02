#include "rotation_utils.h"

// global variables to store the changing angle values
double left_angle = DEFAULT_LEFT_ANGLE;
double right_angle = DEFAULT_RIGHT_ANGLE;

/**
 * @brief Callback function to handle the "activate" signal of the GtkEntry widgets.
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
void rotate_pixbuf(GdkPixbuf *src_pixbuf, GdkPixbuf *dst_pixbuf, double angle)
{
    int width_src = gdk_pixbuf_get_width(src_pixbuf);
    int height_src = gdk_pixbuf_get_height(src_pixbuf);
    int width_dst = gdk_pixbuf_get_width(dst_pixbuf);
    int height_dst = gdk_pixbuf_get_height(dst_pixbuf);
    double radians = angle * G_PI / 180.0;

    int ncx = width_dst / 2;
    int ncy = height_dst / 2;
    int cx = width_src / 2;
    int cy = height_src / 2;

    gdk_pixbuf_fill(dst_pixbuf, 0x00000000); // Fill with transparent color

    for (int y = 0; y < height_dst; y++)
    {
        for (int x = 0; x < width_dst; x++)
        {
            double src_x = (x - ncx) * cos(-radians) - (y - ncy) * sin(-radians) + cx;
            double src_y = (x - ncx) * sin(-radians) + (y - ncy) * cos(-radians) + cy;

            if (src_x >= 0 && src_x < width_src - 1 && src_y >= 0 && src_y < height_src - 1)
            {
                int src_x_int = (int)src_x;
                int src_y_int = (int)src_y;
                double dx = src_x - src_x_int;
                double dy = src_y - src_y_int;

                for (int c = 0; c < gdk_pixbuf_get_n_channels(src_pixbuf); c++)
                {
                    guchar p1 = gdk_pixbuf_get_pixels(src_pixbuf)[src_y_int * gdk_pixbuf_get_rowstride(src_pixbuf) + src_x_int * gdk_pixbuf_get_n_channels(src_pixbuf) + c];
                    guchar p2 = gdk_pixbuf_get_pixels(src_pixbuf)[src_y_int * gdk_pixbuf_get_rowstride(src_pixbuf) + (src_x_int + 1) * gdk_pixbuf_get_n_channels(src_pixbuf) + c];
                    guchar p3 = gdk_pixbuf_get_pixels(src_pixbuf)[(src_y_int + 1) * gdk_pixbuf_get_rowstride(src_pixbuf) + src_x_int * gdk_pixbuf_get_n_channels(src_pixbuf) + c];
                    guchar p4 = gdk_pixbuf_get_pixels(src_pixbuf)[(src_y_int + 1) * gdk_pixbuf_get_rowstride(src_pixbuf) + (src_x_int + 1) * gdk_pixbuf_get_n_channels(src_pixbuf) + c];

                    guchar value = (guchar)((1 - dx) * (1 - dy) * p1 + dx * (1 - dy) * p2 + (1 - dx) * dy * p3 + dx * dy * p4);
                    gdk_pixbuf_get_pixels(dst_pixbuf)[y * gdk_pixbuf_get_rowstride(dst_pixbuf) + x * gdk_pixbuf_get_n_channels(dst_pixbuf) + c] = value;
                }
            }
        }
    }
}

/**
 * @brief Callback function to rotate the image to the left by 5 degrees.
 * @param widget The widget that triggered the function.
 * @param data Pointer to the image widget to be updated.
 * @note We want to use a custom angle which is not in the enum of GdkPixbufRotation. Thus here we use a custom function to rotate. 31/10/24
 */
void on_rotate_left_clicked(GtkWidget *widget, gpointer data)
{
    (void)data; // Remove unused warning
    GtkWidget *image = (GtkWidget *)g_object_get_data(G_OBJECT(widget), "image-widget");
    if (!GTK_IS_IMAGE(image))
    {
        printf("Failed to retrieve image widget\n");
        return;
    }
    GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(image));
    if (!pixbuf)
    {
        printf("Failed to get pixbuf from image\n");
        return;
    }
    printf("Rotating left by %f degrees\n", left_angle);

    int width = gdk_pixbuf_get_width(pixbuf);
    int height = gdk_pixbuf_get_height(pixbuf);
    GdkPixbuf *rotated_pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, gdk_pixbuf_get_has_alpha(pixbuf), 8, width, height);

    rotate_pixbuf(pixbuf, rotated_pixbuf, left_angle);
    gtk_image_set_from_pixbuf(GTK_IMAGE(image), rotated_pixbuf);
    g_object_unref(rotated_pixbuf);
}
/**
 * @brief Callback function to rotate the image to the right by the specified angle.
 * @param widget The widget that triggered the function.
 * @param data User data passed to the function.
 */
void on_rotate_right_clicked(GtkWidget *widget, gpointer data)
{
    (void)data; // Remove unused warning
    GtkWidget *image = (GtkWidget *)g_object_get_data(G_OBJECT(widget), "image-widget");
    if (!GTK_IS_IMAGE(image))
    {
        printf("Failed to retrieve image widget\n");
        return;
    }
    GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(image));
    if (!pixbuf)
    {
        printf("Failed to get pixbuf from image\n");
        return;
    }
    printf("Rotating right by %f degrees\n", right_angle);

    int width = gdk_pixbuf_get_width(pixbuf);
    int height = gdk_pixbuf_get_height(pixbuf);
    GdkPixbuf *rotated_pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, gdk_pixbuf_get_has_alpha(pixbuf), 8, width, height);

    rotate_pixbuf(pixbuf, rotated_pixbuf, right_angle);
    gtk_image_set_from_pixbuf(GTK_IMAGE(image), rotated_pixbuf);
    g_object_unref(rotated_pixbuf);
}