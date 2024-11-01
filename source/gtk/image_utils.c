#include "image_utils.h"

/**
 * @brief Callback function to change the current image with a new image and pass it to image_to_pixbuf
 * @param widget The widget that triggered the function
 * @param data Pointer to the image widget to be updated
 */
void on_change_image(GtkWidget *widget, gpointer data)
{
    GtkWidget *image_widget = GTK_WIDGET(data);
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;

    dialog = gtk_file_chooser_dialog_new("Open File",
                                         GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                         action,
                                         "_Cancel", GTK_RESPONSE_CANCEL,
                                         "_Open", GTK_RESPONSE_ACCEPT,
                                         NULL);

    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_CANCEL)
    {
        gtk_widget_destroy(dialog);
        return;
    }
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);

        // Load the selected image
        GdkPixbuf *loaded_pixbuf = load_pixbuf(filename);
        g_free(filename);

        // Resize the image if necessary

        int width = gdk_pixbuf_get_width(loaded_pixbuf);
        int height = gdk_pixbuf_get_height(loaded_pixbuf);
        GdkPixbuf *resized_pixbuf = resize_pixbuf(loaded_pixbuf, width, height);

        // Apply white borders to the resized image
        GdkPixbuf *final_pixbuf = calculate_borders(resized_pixbuf, IMAGE_WIDTH, IMAGE_HEIGHT);

        // Display the final image with white borders
        gtk_image_set_from_pixbuf(GTK_IMAGE(image_widget), final_pixbuf);

        // Free the pixbufs
        g_object_unref(loaded_pixbuf);
        g_object_unref(resized_pixbuf);
        g_object_unref(final_pixbuf);
    }

    gtk_widget_destroy(dialog);
}
/**
 * @brief Opens a file saving dialog and returns a GtkImage.
 *
 * @param widget The widget that triggers the dialog.
 * @param data User data passed to the function.
 * @return GtkImage* The image selected in the dialog.
 * @note Only the filename is freed // TODO rework
 */
void *on_save_image(GtkWidget *widget, gpointer data)
{
    /**
     * TODO add gtk_file_chooser_get_create_folders, gets whether file choser will offer to create new folders. See gtk_file_chooser_set_create_folders().

  since: 2.18 */
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
    gint res;
    // TODO understand gtk_widget_get_toplevel
    dialog = gtk_file_chooser_dialog_new("Save File",
                                         GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                         action,
                                         "_Cancel", GTK_RESPONSE_CANCEL,
                                         "_Save", GTK_RESPONSE_ACCEPT,
                                         NULL);
    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_CANCEL)
    {
        gtk_widget_destroy(dialog);
        return NULL;
    }
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);
        // Save the image
        GdkPixbuf *pixbuf = image_to_pixbuf(GTK_IMAGE(data));
        if (pixbuf != NULL)
        {
            /**TODO there must be an elegant way
            Ensure the filename has a .png extension*/
            const char *extension = ".png";
            char *filename_with_extension = g_strconcat(filename, extension, NULL);
            gdk_pixbuf_save(pixbuf, filename_with_extension, "png", NULL, NULL);
            g_free(filename_with_extension);
        }
        gtk_widget_destroy(dialog);
        g_free(filename);
    }
    return NULL;
}
/**
 * @brief Loads a pixbuf from a file.
 * @param file_path The path to the file.
 * @return A pointer to the loaded pixbuf.
 * @note The pixbuf is not freed
 */
GdkPixbuf *load_pixbuf(const char *file_path)
{
    return gdk_pixbuf_new_from_file(file_path, NULL);
}

/**
 * @brief Displays a pixbuf in an image widget.
 * @param image_widget The image widget to display the pixbuf in (implicit cast to GtkImage)
 * @param pixbuf The pixbuf to display.
 * @note The pixbuf is not freed
 */
void display_pixbuf(GtkWidget *image_widget, GdkPixbuf *pixbuf)
{
    gtk_image_set_from_pixbuf(GTK_IMAGE(image_widget), pixbuf);
}

/**
 * @brief Resizes a pixbuf to fit within the specified dimensions.
 * @param pixbuf The pixbuf to resize.
 * @param width The maximum width of the resized pixbuf.
 * @param height The maximum height of the resized pixbuf.
 * @return A pointer to the resized pixbuf with the adequate aspect ratio and borders.
 * @note The pixbuf is not freed
 */
GdkPixbuf *resize_pixbuf(GdkPixbuf *pixbuf, int width, int height)
{ // TODOOOOOO FIX !
    int original_width = gdk_pixbuf_get_width(pixbuf);
    int original_height = gdk_pixbuf_get_height(pixbuf);

    // Calculate the scaling factor to maintain the aspect ratio
    double scale = MIN((double)width / original_width, (double)height / original_height);

    int new_width = (int)(original_width * scale);
    int new_height = (int)(original_height * scale);

    return gdk_pixbuf_scale_simple(pixbuf, new_width, new_height, GDK_INTERP_BILINEAR); // Bilinear interpolation to resize the image
}

/**
 * @brief Creates a alpha-background pixbuf of the specified dimensions.
 * @param width The width of the pixbuf.
 * @param height The height of the pixbuf.
 * @return A pointer to the new white pixbuf.
 * @note The pixbuf is not freed. You should call it with the sizes as macros for consistency.
 */
GdkPixbuf *create_alpha_pixbuf(int width, int height)
{
    GdkPixbuf *pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, width, height);
    guchar *pixels = gdk_pixbuf_get_pixels(pixbuf);
    int rowstride = gdk_pixbuf_get_rowstride(pixbuf);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            guchar *pixel = pixels + y * rowstride + x * 4;
            pixel[0] = 0; // Red
            pixel[1] = 0; // Green
            pixel[2] = 0; // Blue
            pixel[3] = 0; // Alpha
        }
    }

    return pixbuf;
}

// Function to add borders to a pixbuf to fit within the specified dimensions
GdkPixbuf *calculate_borders(GdkPixbuf *pixbuf, int width, int height)
{
    int original_width = gdk_pixbuf_get_width(pixbuf);
    int original_height = gdk_pixbuf_get_height(pixbuf);

    GdkPixbuf *white_pixbuf = create_alpha_pixbuf(width, height);
    int offset_x = (width - original_width) / 2;
    int offset_y = (height - original_height) / 2;

    gdk_pixbuf_copy_area(pixbuf, 0, 0, original_width, original_height, white_pixbuf, offset_x, offset_y);

    return white_pixbuf;
}
