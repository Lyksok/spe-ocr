#include "image_utils.h"

/**
 * @brief Processes and displays a pixbuf in the specified image widget.
 * @param image_widget The image widget to display the pixbuf in.
 * @param pixbuf The pixbuf to process and display.
 */
void process_and_display_image(GtkWidget *image_widget, GdkPixbuf *pixbuf)
{
    printf("Entered process_and_display_image\n");

    // Create a new pixbuf with alpha borders and center the resized pixbuf within it
    GdkPixbuf *final_pixbuf = resize_with_borders(pixbuf);
    if (!final_pixbuf)
    {
        printf("Failed to create pixbuf with alpha borders\n");
        return;
    }
    printf("Final pixbuf with alpha borders created\n");

    // Display the final image with alpha borders
    gtk_image_set_from_pixbuf(GTK_IMAGE(image_widget), final_pixbuf);
    printf("Image set in image_widget\n");

    // Free the final pixbuf
    g_object_unref(final_pixbuf);
    printf("Freed final pixbuf\n");
}

void on_change_image(GtkWidget *widget, gpointer data)
{
    GtkWidget *image_widget = GTK_WIDGET(data);
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;

    dialog = gtk_file_chooser_dialog_new("Open File",
                                         GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                         action,
                                         "_Cancel",
                                         GTK_RESPONSE_CANCEL,
                                         "_Open",
                                         GTK_RESPONSE_ACCEPT,
                                         NULL);

    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);
        GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
        if (pixbuf)
        {
            process_and_display_image(image_widget, pixbuf);
            g_object_unref(pixbuf);
        }
        g_free(filename);
    }

    gtk_widget_destroy(dialog);
}

void *on_save_image(GtkWidget *widget, gpointer data)
{
    GtkWidget *image_widget = GTK_WIDGET(data);
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
    gint res;

    dialog = gtk_file_chooser_dialog_new("Save File",
                                         GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                         action,
                                         "_Cancel",
                                         GTK_RESPONSE_CANCEL,
                                         "_Save",
                                         GTK_RESPONSE_ACCEPT,
                                         NULL);

    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);
        GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(image_widget));
        if (pixbuf)
        {
            gdk_pixbuf_save(pixbuf, filename, "png", NULL, NULL);
        }
        g_free(filename);
    }

    gtk_widget_destroy(dialog);
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

    double scale = fmin((double)max_width / original_width, (double)max_height / original_height);

    int new_width = (int)(original_width * scale);
    int new_height = (int)(original_height * scale);

    // Create a new pixbuf for the resized image
    GdkPixbuf *resized_pixbuf = gdk_pixbuf_new(gdk_pixbuf_get_colorspace(pixbuf),
                                               gdk_pixbuf_get_has_alpha(pixbuf),
                                               gdk_pixbuf_get_bits_per_sample(pixbuf),
                                               new_width, new_height);

    // Scale the original pixbuf into the new pixbuf
    gdk_pixbuf_scale(pixbuf, resized_pixbuf,
                     0, 0, new_width, new_height,
                     0, 0, (double)new_width / original_width, (double)new_height / original_height,
                     GDK_INTERP_BILINEAR);

    return resized_pixbuf;
}

/**
 * @brief Resizes a pixbuf to fit within the diagonal length and creates a new pixbuf with alpha borders.
 * @param pixbuf The original pixbuf to resize and center.
 * @return A pointer to the new pixbuf with alpha borders.
 */
GdkPixbuf *resize_with_borders(GdkPixbuf *pixbuf)
{
    int original_width = gdk_pixbuf_get_width(pixbuf);
    int original_height = gdk_pixbuf_get_height(pixbuf);

    // Step 1: Calculate the diagonal length
    int diagonal_length = (int)sqrt(original_width * original_width + original_height * original_height);

    // Step 2: Add borders to make the image a square with side length equal to the diagonal length
    GdkPixbuf *pixbuf_1 = create_pixbuf_with_alpha_borders(pixbuf, diagonal_length, diagonal_length);
    if (!pixbuf_1)
    {
        printf("Failed to create pixbuf with alpha borders\n");
        return NULL;
    }

    // Step 3: Resize the squared image to fit within MAX_WIDTH and MAX_HEIGHT
    GdkPixbuf *resized_pixbuf = resize_pixbuf(pixbuf_1, MAX_WIDTH, MAX_HEIGHT);
    g_object_unref(pixbuf_1);
    if (!resized_pixbuf)
    {
        printf("Failed to resize pixbuf\n");
        return NULL;
    }

    // Step 4: Add borders to the resized image to fit within MAX_WIDTH and MAX_HEIGHT
    GdkPixbuf *final_pixbuf = create_pixbuf_with_alpha_borders(resized_pixbuf, MAX_WIDTH, MAX_HEIGHT);
    g_object_unref(resized_pixbuf);
    if (!final_pixbuf)
    {
        printf("Failed to create final pixbuf with alpha borders\n");
        return NULL;
    }

    return final_pixbuf;
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