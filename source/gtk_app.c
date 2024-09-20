#include <gtk/gtk.h>

/**
 * @brief Opens a file chooser dialog to select and load an image file
 * 
 * @param widget The widget that triggered the function
 * @param data Additional data, typically the parent window
 * @return GtkImage* The loaded image widget, or NULL if no file was selected
 */
GtkImage *load_image_from_file(GtkWidget *widget, gpointer data)
{
  GtkWidget *dialog;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN; // it is used to specify the type of "action" that the fc performs
  gint res;
  GtkWidget *image = NULL;

  // da file chooser dialog is a dialog box used for "File Open" or "File Save" commands
  dialog = gtk_file_chooser_dialog_new("Open Image",
                                       GTK_WINDOW(data),
                                       action,
                                       "_Cancel",
                                       GTK_RESPONSE_CANCEL,
                                       "_Open",
                                       GTK_RESPONSE_ACCEPT,
                                       NULL);

  res = gtk_dialog_run(GTK_DIALOG(dialog)); // returns the response id of the button clicked by the user
  if (res == GTK_RESPONSE_ACCEPT) // if the user chooses to open a file
  {
    char *filename;
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog); // gtkfilechooser is a widget for choosing files
    filename = gtk_file_chooser_get_filename(chooser); // gets chosen absolute path
    image = gtk_image_new_from_file(filename); // creates a new image widget from the file
    g_free(filename); // frees the memory occupied by filename, bcs we don't need it anymore
  }

  gtk_widget_destroy(dialog);
  return image;
}
/**
 * @brief Creates the menu bar for the given GTK window.
 *
 * This function initializes menu bar for the specified
 * GTK window. It adds the menu items to load and save image from a file explorer.
 *
 * @param window A pointer to the GTK window where the menu bar will be added.
 */
void create_menu_bar(GtkWidget *window)
{
  GtkWidget *menu_bar;
  GtkWidget *file_menu;
  GtkWidget *file_menu_item;
  GtkWidget *load_menu_item;
  GtkWidget *save_menu_item;

  // Create a menu bar
  menu_bar = gtk_menu_bar_new();

  // Create a file menu and add it to the menu bar
  file_menu = gtk_menu_new();
  file_menu_item = gtk_menu_item_new_with_label("File");
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_menu_item), file_menu);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file_menu_item);

  // Create load menu item in file_menu
  load_menu_item = gtk_menu_item_new_with_label("Open Image");
  g_signal_connect(load_menu_item, "activate", G_CALLBACK(load_image_from_file), window);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), load_menu_item);

  // Create save menu item in file_menu
  save_menu_item = gtk_menu_item_new_with_label("Save Image as");
  g_signal_connect(save_menu_item, "activate", G_CALLBACK(save_image_to_file), NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), save_menu_item);

  // Add menu_bar to window
  gtk_container_add(GTK_CONTAINER(window), menu_bar);
}

/**
 * @brief Creates a GtkImage widget.
 *
 * This function initializes and returns a new GtkImage widget.
 *
 * @param widget A pointer to the GtkWidget.
 * @param data A gpointer to any additional data needed for image creation.
 * @return A pointer to the new GtkImage.
 */
GtkImage* create_image(GtkWidget *widget, gpointer data){
  GtkImage *image = load_image_from_file(widget, data);
  if (image != NULL){
    gtk_container_add(GTK_CONTAINER(widget), GTK_WIDGET(image));
    gtk_widget_show(GTK_WIDGET(image));
  }
  return image;
}
// ***** CREATE IMAGE MANIPULATION BUTTONS *****
GtkWidget *rotation_buttons(){}


/**
 * @brief Activates the GTK application OCR !
 *
 * @param app A pointer to the GtkApplication instance.
 */
static void activate(GtkApplication *app)
{
  GtkWidget *window;
  GtkWidget *label;

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "OCR App for crosswords by Fanette Saury, Jans Guillopé and Lise Suzanne");
  gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);

  // Create a label widget

  // Add the label to the window
  gtk_container_add(GTK_CONTAINER(window), label);

  gtk_widget_show_all(window);
}

int main(int argc, char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new("org.gtk.OCR", G_APPLICATION_DEFAULT_FLAGS); // creates a new gtk app instance
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL); // connects the "activate" signal to the "activate" function
  status = g_application_run(G_APPLICATION(app), argc, argv); // run app
  g_object_unref(app); // decrements the reference count of the object ie freeds the memory

  return status;
}
