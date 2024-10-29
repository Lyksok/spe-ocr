#include <gtk/gtk.h>

// Callback functions begin with "on_"
// All function names are in snake_case
// reminder : GTK_WIDGET is a macro that casts the pointer to a GtkWidget, just conventional


/**
 * @brief Opens a file chooser dialog to select and load an image file
 * @param widget The widget that triggered the function
 * @param data for the parent window
 * @return return image absolute path as a string
 */
char* open_file_dialog(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog;
    GtkFileChooser *chooser;
    GtkWidget *window = GTK_WIDGET(data);
    char *filename = NULL;

    // Create a file chooser dialog
    dialog = gtk_file_chooser_dialog_new("Open Image File",
                                         GTK_WINDOW(window),
                                         GTK_FILE_CHOOSER_ACTION_OPEN,
                                         "_Cancel", GTK_RESPONSE_CANCEL,
                                         "_Open", GTK_RESPONSE_ACCEPT,
                                         NULL);

    // Run the dialog and check the response
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser); // Get the selected file ABSOLUTE path

        // Load the image
        GtkWidget *image = gtk_image_new_from_file(filename);
        if (image) {
            // Add the image to the window or a container
            gtk_container_add(GTK_CONTAINER(window), image);
            gtk_widget_show_all(window);
        } else {
            g_print("Failed to load image: %s\n", filename);
            g_free(filename);
            filename = NULL;
        }
    }

    gtk_widget_destroy(dialog);
    return filename;
}

GtkImage *save_file_dialog(GtkWidget *widget, gpointer data)
{
  // GtkWidget *dialog;
  // GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE; // "action" type performed by fc
  // int res;
  GtkWidget *image = NULL;
  // TODO
  return GTK_IMAGE(image);
}

/**
 * @brief Creates a GtkImage widget.
 * Initializes and returns a new GtkImage widget.
 * @param widget The triggering GtkWidget.
 * @param data Additional data for image creation.
 * @return A pointer to the new GtkImage.
 */
GtkImage *init_image_widget(GtkWidget *parent, const char *sample_image_path)
{
  GtkImage *image = GTK_IMAGE(gtk_image_new_from_file(sample_image_path));
  // gtk_widget_set_size_request(GTK_WIDGET(image), 450, 450); // Set the size of the placeholder image

    return image;
}

/**
 * @brief Callback function to load an image and update the image widget
 * @param widget The widget that triggered the function
 * @param data Pointer to the image widget to be updated
 */
void on_load_image_from_file(GtkWidget *widget, gpointer data)
{
    const char *filename = (const char *)data;

    if (filename != NULL)
    {
        GtkWidget *parent = gtk_widget_get_parent(GTK_WIDGET(widget));
        if (GTK_IS_WIDGET(parent))
        {
            // Load the new image
            GtkWidget *new_image = gtk_image_new_from_file(filename);
            if (new_image != NULL)
            {
                // Remove the old image
                gtk_widget_destroy(GTK_WIDGET(widget));

                // Add the new image to the parent container
                gtk_box_pack_start(GTK_BOX(parent), new_image, TRUE, TRUE, 0);
                gtk_widget_show_all(new_image);
            }
    // DEBUG : Warnings
            else
            {
                g_warning("Failed to load image: %s", filename);
            }
        }
        else
        {
            g_warning("Parent widget is not valid.");
        }
    }
    else
    {
        g_warning("Filename is NULL.");
    }
}
/**
 * @brief Creates a button widget.
 *
 * This function initializes and returns a new GtkButton widget.
 *
 * @param label The label for the button.
 * @param callback The callback function to be called when the button is clicked.
 * @param data A gpointer to any additional data needed for the callback.
 * @return A pointer to the new GtkButton.
 */
GtkWidget *init_button(const char *label, GCallback callback, gpointer data)
{
  GtkWidget *button = gtk_button_new_with_label(label);
  if (callback != NULL)
    // connects the "clicked" signal to the callback function
    g_signal_connect(button, "clicked", callback, data);
  return button;
}
/**
 * @brief Creates the menu bar for the given GTK window.
 * This function initializes menu bar for the specified
 * GTK window. It adds the menu items to load and save image from a file explorer.
 * @param window A pointer to the GTK window where the menu bar will be added.
 */
GtkWidget *init_menu_bar(GtkWidget *window)
{
  GtkWidget *menu_bar, *file_menu, *file_menu_item, *load_menu_item, *save_menu_item;

  // Creates a menu bar
  menu_bar = gtk_menu_bar_new();

  // Creates a file menu => menu bar
  file_menu = gtk_menu_new();
  file_menu_item = gtk_menu_item_new_with_label("File");
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_menu_item), file_menu);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file_menu_item);

  // Creates load menu item => file_menu
  load_menu_item = gtk_menu_item_new_with_label("Load Image");
  g_signal_connect(load_menu_item, "activate", G_CALLBACK(on_load_image_from_file), window);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), load_menu_item);

  // Creates save menu item => file_menu
  save_menu_item = gtk_menu_item_new_with_label("Save Image");
  g_signal_connect(save_menu_item, "activate", G_CALLBACK(save_file_dialog), window);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), save_menu_item);
  // Add menu_bar => window
  return menu_bar;
}
/**
 * @brief Activates the app. Precisely, it lays out the widgets and connects the signals.
 * @param app A pointer to the GtkApplication instance.
 */
static void activate(GtkApplication *app)
{
    GtkWidget *window, *image, *button, *menu_bar, *vbox, *hbox, *vbox_buttons, *empty_box;
    // declare pointers on the objects to be initialised
    // ORDER OF WIDGETS DECLARATION IS DETEMINANT FOR THE ORDER OF DISPLAY !!!
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "OCR App for Crosswords");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    gtk_window_set_resizable(GTK_WINDOW(window), TRUE);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    menu_bar = init_menu_bar(window);
    gtk_box_pack_start(GTK_BOX(vbox), menu_bar, FALSE, FALSE, 0);

    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 0);

    // Initialize the image widget with a sample image
    image = GTK_WIDGET(init_image_widget(hbox, "images/abstract_background.jpg"));

    gtk_widget_set_halign(image, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(image, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(hbox), image, TRUE, TRUE, 0);

    vbox_buttons = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(hbox), vbox_buttons, FALSE, FALSE, 0);

    // Handy loop to add multiple buttons to the right column
    const char *button_labels[] = {"Grayscale", "Binarize", "Rotate"};
    for (int i = 0; i < sizeof(button_labels) / sizeof(button_labels[0]); i++)
    {
        button = init_button(button_labels[i], NULL, NULL);
        // gtk_widget_set_size_request(button, 100, 50);
        gtk_box_pack_start(GTK_BOX(vbox_buttons), button, FALSE, FALSE, 0);
    }

     // Proportional parameters to resize the widgets if the window is resized
    gtk_widget_set_hexpand(image, TRUE);
    gtk_widget_set_vexpand(image, TRUE);
    gtk_widget_set_hexpand(vbox_buttons, TRUE);
    gtk_widget_set_vexpand(vbox_buttons, TRUE);

    gtk_widget_show_all(window);
}


/**
 * @brief Main function to run the GTK application.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return int Status code.
 */
int main(int argc, char **argv)
{
  GtkApplication *app;
  int status;

  /*  Window creation  */
  // gtk_init() called in gtk_application_new '
  app = gtk_application_new("org.gtk.OCR", G_APPLICATION_DEFAULT_FLAGS); // creates a new gtk app instance

  /*  Signal connections */
  // "activate" signal connection
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL); // connects the "activate" signal to the "activate" function

 /*  Run event loop  */
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}