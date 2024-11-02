#include "image_utils.h"

/**
 * @brief Processes and displays a pixbuf in the specified image widget.
 * @param image_widget The image widget to display the pixbuf in.
 * @param pixbuf The pixbuf to process and display.
 */
void process_and_display_image(GtkWidget *image_widget, GdkPixbuf *pixbuf)
{
    printf("Entered process_and_display_image\n");

    // Resize the image if necessary
    int width = gdk_pixbuf_get_width(pixbuf);
    int height = gdk_pixbuf_get_height(pixbuf);
    printf("Original image dimensions: width=%d, height=%d\n", width, height);

    GdkPixbuf *resized_pixbuf = resize_pixbuf(pixbuf, MAX_WIDTH, MAX_HEIGHT);
    if (!resized_pixbuf)
    {
        printf("Failed to resize pixbuf\n");
        return;
    }
    printf("Resized pixbuf created\n");

    // Create a new pixbuf with alpha borders and center the resized pixbuf within it
    GdkPixbuf *final_pixbuf = create_pixbuf_with_alpha_borders(resized_pixbuf, MAX_WIDTH, MAX_HEIGHT);
    if (!final_pixbuf)
    {
        printf("Failed to create pixbuf with alpha borders\n");
        g_object_unref(resized_pixbuf);
        return;
    }
    printf("Final pixbuf with alpha borders created\n");

    // Display the final image with alpha borders
    gtk_image_set_from_pixbuf(GTK_IMAGE(image_widget), final_pixbuf);
    printf("Image set in image_widget\n");

    // Free the pixbufs
    g_object_unref(resized_pixbuf);
    g_object_unref(final_pixbuf);
    printf("Freed resized and final pixbufs\n");
}

/**
 * @brief Callback function to change the current image with a new image.
 * @param widget The widget that triggered the function.
 * @param data Pointer to the image widget to be updated.
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
        GdkPixbuf *loaded_pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
        g_free(filename);

        if (loaded_pixbuf)
        {
            // Process and display the selected image
            process_and_display_image(image_widget, loaded_pixbuf);
            g_object_unref(loaded_pixbuf);
        }
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
 * @brief Resizes a pixbuf to fit within the specified dimensions while maintaining the aspect ratio.
 * @param pixbuf The pixbuf to resize.
 * @param max_width The maximum width of the resized pixbuf.
 * @param max_height The maximum height of the resized pixbuf.
 * @return A pointer to the resized pixbuf with the adequate aspect ratio.
 * @note The pixbuf is not freed.
 */
GdkPixbuf *resize_pixbuf(GdkPixbuf *pixbuf, int max_width, int max_height)
{
    int original_width = gdk_pixbuf_get_width(pixbuf);
    int original_height = gdk_pixbuf_get_height(pixbuf);

    double scale = MIN((double)max_width / original_width, (double)max_height / original_height);

    int new_width = (int)(original_width * scale);
    int new_height = (int)(original_height * scale);

    // Resize the pixbuf while maintaining the aspect ratio
    GdkPixbuf *resized_pixbuf = gdk_pixbuf_scale_simple(pixbuf, new_width, new_height, GDK_INTERP_BILINEAR);

    return resized_pixbuf;
}

/**
 * @brief Creates a new pixbuf with alpha borders and centers the original pixbuf within it.
 * @param pixbuf The original pixbuf to center.
 * @param width The width of the new pixbuf.
 * @param height The height of the new pixbuf.
 * @return A pointer to the new pixbuf with alpha borders.
 */
GdkPixbuf *create_pixbuf_with_alpha_borders(GdkPixbuf *pixbuf, int width, int height)
{
    int original_width = gdk_pixbuf_get_width(pixbuf);
    int original_height = gdk_pixbuf_get_height(pixbuf);

    GdkPixbuf *final_pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, width, height);
    gdk_pixbuf_fill(final_pixbuf, 0x00000000); // Fill with transparent color

    int offset_x = (width - original_width) / 2;
    int offset_y = (height - original_height) / 2;

    gdk_pixbuf_copy_area(pixbuf, 0, 0, original_width, original_height, final_pixbuf, offset_x, offset_y);

    return final_pixbuf;
}