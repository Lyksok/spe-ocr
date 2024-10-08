#include <gtk/gtk.h>

// reminder : GTK_WIDGET is a macro that casts the pointer to a GtkWidget, just conventional
// TODO : refactor load image names ...

/**
 * @brief Opens a file chooser dialog to select and load an image file
 * @param widget The widget that triggered the function
 * @param data for the parent window
 * @return loadd GtkImage widgt or NULL if no image was loaded
 */
GtkImage *load_image_from_file(GtkWidget *widget, gpointer data)
{
  GtkWidget *dialog;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN; // "action" type performed by fc
  int res;
  GtkWidget *image = NULL;

  // fc = dialog box used for "File Open" or "File Save" actions
  dialog = gtk_file_chooser_dialog_new("Open Image",
                                       GTK_WINDOW(data),
                                       action,
                                       "_Cancel",
                                       GTK_RESPONSE_CANCEL,
                                       "_Open",
                                       GTK_RESPONSE_ACCEPT,
                                       NULL);

  res = gtk_dialog_run(GTK_DIALOG(dialog)); // response id of the button clicked by the user
  if (res == GTK_RESPONSE_ACCEPT)
  {
    char *filename;
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog); // widget for choosing files
    filename = gtk_file_chooser_get_filename(chooser);  // get filename from fc
    image = gtk_image_new_from_file(filename);  // creates a new image widget from the file
    g_free(filename); // free filename memory
  }

  gtk_widget_destroy(dialog);
  return GTK_IMAGE(image);
}

GtkImage *save_image_from_file(GtkWidget *widget, gpointer data)
{
  GtkWidget *dialog;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE; // "action" type performed by fc
  int res;
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
GtkImage *create_image(GtkWidget *widget, gpointer data)
{
  GtkImage *image = GTK_IMAGE(gtk_image_new());
  gtk_widget_set_size_request(GTK_WIDGET(image), 450, 450); // Set the size of the placehollder image
  return image;
}

/**
 * @brief Callback function to load an image and update the image widget
 * @param widget The widget that triggered the function
 * @param data Pointer to the image widget to be updated
 */
void callback_load_image(GtkWidget *widget, gpointer data)
{
  GtkWidget *old = gtk_widget_get_parent(GTK_WIDGET(data));
  GtkImage *new = load_image_from_file(widget, old); // type is GtkImage instead of GtkWidget => bcs of the return type of load_image_from_file
  // TODO understand 2nd parameter cf docu
  if (new != NULL)
  {
    gtk_widget_destroy(GTK_WIDGET(data));
    gtk_box_pack_start(GTK_BOX(old), GTK_WIDGET(new), TRUE, TRUE, 0); // TODO understand better relationship parent-child btw widgets
    gtk_widget_show_all(GTK_WIDGET(new));
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
GtkWidget *create_button(const char *label, GCallback callback, gpointer data)
{
  GtkWidget *button = gtk_button_new_with_label(label);
  g_signal_connect(button, "clicked", callback, data);
  return button;
}
/**
 * @brief Creates the menu bar for the given GTK window.
 * This function initializes menu bar for the specified
 * GTK window. It adds the menu items to load and save image from a file explorer.
 * @param window A pointer to the GTK window where the menu bar will be added.
 */
GtkWidget *create_menu_bar(GtkWidget *window)
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
  load_menu_item = gtk_menu_item_new_with_label("Open Image");
  g_signal_connect(load_menu_item, "activate", G_CALLBACK(callback_load_image), window);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), load_menu_item);

  // Creates save menu item => file_menu
  save_menu_item = gtk_menu_item_new_with_label("Save Image");
  g_signal_connect(save_menu_item, "activate", G_CALLBACK(save_image_from_file), window);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), save_menu_item);
  // Add menu_bar => window
  return menu_bar;
}
/**
 * @brief Activates the GTK application OCR !
 * @param app A pointer to the GtkApplication instance.
 */
static void activate(GtkApplication *app)
{
  GtkWidget *window, *image, *button, *menu_bar, *vbox, *hbox, *vbox_buttons, *empty_box;
  // ORDER OF WIDGETS DECLARATION IS DETEMINANT FOR THE ORDER OF DISPLAY !!!
  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "OCR App for crosswords by Caroline Deliere, Fanette Saury, Jans Guillopé and Lise Suzanne");
  gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
  gtk_window_set_resizable(GTK_WINDOW(window), TRUE);

  vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  image = GTK_WIDGET(create_image(window, NULL));

  menu_bar = create_menu_bar(window);
  gtk_box_pack_start(GTK_BOX(vbox), menu_bar, FALSE, FALSE, 0);

  hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 0);

  gtk_widget_set_halign(image, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(image, GTK_ALIGN_CENTER);
  gtk_box_pack_start(GTK_BOX(hbox), image, TRUE, TRUE, 0);

  vbox_buttons = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_box_pack_start(GTK_BOX(hbox), vbox_buttons, FALSE, FALSE, 0);

  empty_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0); // helper for layout

  // empty_boxes + button = vbox
  gtk_box_pack_start(GTK_BOX(vbox_buttons), empty_box, TRUE, TRUE, 0);
  button = create_button("Grayscale", NULL, NULL);
  gtk_widget_set_size_request(button, 100, 50);
  gtk_box_pack_start(GTK_BOX(vbox_buttons), button, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox_buttons), empty_box, TRUE, TRUE, 0);

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

  app = gtk_application_new("org.gtk.OCR", G_APPLICATION_DEFAULT_FLAGS); // creates a new gtk app instance
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);  // connects the "activate" signal to the "activate" function
  status = g_application_run(G_APPLICATION(app), argc, argv); // run app
  g_object_unref(app);  // decrements reference count of the gtk_app ie freeds the memory bcs it's last instruction

  return status;
}