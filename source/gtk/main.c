#include "main.h"

/**
 * @brief Callback function to destroy the main window.
 */
void on_window_main_destroy(GtkWidget *widget, gpointer data)
{
  gtk_main_quit();
}
/**
 * @brief Callback function to destroy a widget.
 * @param widget The widget to be destroyed.
 * @return gboolean Always returns FALSE to stop the timeout.
 */
gboolean destroy_widget(gpointer widget)
{
  if (GTK_IS_WIDGET(widget))
  {
    GList *children, *p;
    children = gtk_container_get_children(GTK_CONTAINER(widget)); // list
    for (p = children; p != NULL; p = g_list_next(p))
    {
      gtk_widget_destroy(GTK_WIDGET(p->data));
    }
    g_list_free(children);                  // free the handy list
    gtk_widget_destroy(GTK_WIDGET(widget)); // free parent
  }
  return FALSE;
}

// /**
//  * @brief Creates a button widget.
//  *
//  * This function initializes and returns a new GtkButton widget.
//  *
//  * @param label The label for the button.
//  * @param callback The callback function to be called when the button is
//  * clicked.
//  * @param data A gpointer to any additional data needed for the callback.
//  * @return A pointer to the new GtkButton.
//  */
// GtkWidget *init_button(const char *label, GCallback callback, gpointer data)
// {
//   GtkWidget *button = gtk_button_new_with_label(label);
//   if (callback != NULL)
//     // connects the "clicked" signal to the callback function
//     g_signal_connect(button, "clicked", callback, data);

//   // Align the text to the left
//   GtkWidget *txt_widget = gtk_bin_get_child(GTK_BIN(button));
//   gtk_widget_set_halign(txt_widget, GTK_ALIGN_START);

//   // Set the image widget as data on the button
//   g_object_set_data(G_OBJECT(button), "image-widget", data);

//   return button;
// }
// /**
//  * @brief Creates the menu bar for the given GTK window.
//  * This function initializes menu bar for the specified
//  * GTK window. It adds the menu items to load and save image from a file
//  * explorer.
//  * @param image_widget The image widget to be updated when clicking on the
//  * submenus.
//  */
// GtkWidget *init_menu_bar(GtkWidget *image_widget)
// {
//   GtkWidget *menu_bar, *file_menu, *solver_menu, *file_menu_item, *load_menu_item,
//       *save_menu_item, *solver_menu_item;

//   // Creates a menu bar
//   menu_bar = gtk_menu_bar_new();
//   // Creates the menus
//   file_menu = gtk_menu_new();
//   file_menu_item = gtk_menu_item_new_with_label("📁 File");
//   gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_menu_item), file_menu);
//   gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file_menu_item);

//   // Creates load menu item => file_menu
//   load_menu_item = gtk_menu_item_new_with_label("Load Image");
//   g_signal_connect(load_menu_item, "activate", G_CALLBACK(on_change_image),
//                    image_widget);
//   gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), load_menu_item);

//   // Creates save menu item => file_menu
//   save_menu_item = gtk_menu_item_new_with_label("Save Image");
//   g_signal_connect(save_menu_item, "activate", G_CALLBACK(on_save_image),
//                    image_widget);
//   gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), save_menu_item);

//   solver_menu = gtk_menu_new();
//   solver_menu_item = gtk_menu_item_new_with_label("🧩 Solver");
//   gtk_menu_item_set_submenu(GTK_MENU_ITEM(solver_menu_item), solver_menu);
//   gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), solver_menu_item);

//   // Creates solver menu item => solver_menu
//   solver_menu_item = gtk_menu_item_new_with_label("Solver");
//   g_signal_connect(solver_menu_item, "activate", G_CALLBACK(create_solver_window), NULL);
//   gtk_menu_shell_append(GTK_MENU_SHELL(solver_menu), solver_menu_item);

//   // Grayscale processing menu
//   GtkWidget *grayscale_menu = gtk_menu_new();
//   GtkWidget *grayscale_menu_item = gtk_menu_item_new_with_label("🌑 Grayscale Processing");
//   gtk_menu_item_set_submenu(GTK_MENU_ITEM(grayscale_menu_item), grayscale_menu);
//   gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), grayscale_menu_item);

//   GtkWidget *grayscale_item = gtk_menu_item_new_with_label("Grayscale");
//   g_signal_connect(grayscale_item, "activate", G_CALLBACK(on_grayscale_clicked), image_widget);
//   gtk_menu_shell_append(GTK_MENU_SHELL(grayscale_menu), grayscale_item);

//   GtkWidget *contrast_item = gtk_menu_item_new_with_label("Contrast Amplification");
//   g_signal_connect(contrast_item, "activate", G_CALLBACK(on_contrast_amplification_clicked), image_widget);
//   gtk_menu_shell_append(GTK_MENU_SHELL(grayscale_menu), contrast_item);

//   GtkWidget *median_filter_item = gtk_menu_item_new_with_label("Median Filter");
//   g_signal_connect(median_filter_item, "activate", G_CALLBACK(on_median_filter_clicked), image_widget);
//   gtk_menu_shell_append(GTK_MENU_SHELL(grayscale_menu), median_filter_item);

//   GtkWidget *gaussian_filter_item = gtk_menu_item_new_with_label("Gaussian Filter");
//   g_signal_connect(gaussian_filter_item, "activate", G_CALLBACK(on_gaussian_filter_clicked), image_widget);
//   gtk_menu_shell_append(GTK_MENU_SHELL(grayscale_menu), gaussian_filter_item);

//   // Thresholding menu
//   GtkWidget *thresholding_menu = gtk_menu_new();
//   GtkWidget *thresholding_menu_item = gtk_menu_item_new_with_label("📈 Thresholding");
//   gtk_menu_item_set_submenu(GTK_MENU_ITEM(thresholding_menu_item), thresholding_menu);
//   gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), thresholding_menu_item);

//   GtkWidget *average_item = gtk_menu_item_new_with_label("Average");
//   g_signal_connect(average_item, "activate", G_CALLBACK(on_average_thresholding_clicked), image_widget);
//   gtk_menu_shell_append(GTK_MENU_SHELL(thresholding_menu), average_item);

//   GtkWidget *adaptive_item = gtk_menu_item_new_with_label("Adaptive (Local)");
//   g_signal_connect(adaptive_item, "activate", G_CALLBACK(on_adaptive_thresholding_clicked), image_widget);
//   gtk_menu_shell_append(GTK_MENU_SHELL(thresholding_menu), adaptive_item);

//   GtkWidget *otsu_item = gtk_menu_item_new_with_label("Otsu (Global)");
//   g_signal_connect(otsu_item, "activate", G_CALLBACK(on_otsu_thresholding_clicked), image_widget);
//   gtk_menu_shell_append(GTK_MENU_SHELL(thresholding_menu), otsu_item);

//   // Detection menu
//   // TODO

//   // Segmentation menu
//   // TODO

//   // Manual mode menu
//   // TODO think about it
//   return menu_bar;
// }

// /**
//  * @brief Activates the app. Precisely, it lays out the widgets and connects
//  * the signals.
//  * @param app A pointer to the GtkApplication instance.
//  */
// static void activate(GtkApplication *app, gpointer user_data)
// {
//   GtkBuilder *builder;
//   GtkWidget *window;
//   GtkCssProvider *css_provider;
//   GError *error = NULL;

//   builder = gtk_builder_new();
//   if (!gtk_builder_add_from_file(builder, "source/gtk/layout/layout.ui", &error))
//   {
//     g_warning("%s", error->message);
//     g_clear_error(&error);
//     return;
//   }

//   window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
//   gtk_window_set_application(GTK_WINDOW(window), app);

//   // Load CSS
//   css_provider = gtk_css_provider_new();
//   gtk_css_provider_load_from_path(css_provider, "source/gtk/style/style.css", NULL);
//   gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
//                                             GTK_STYLE_PROVIDER(css_provider),
//                                             GTK_STYLE_PROVIDER_PRIORITY_USER);

//   // Connect signals
//   g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

//   // Get references to other widgets
//   GtkWidget *main_box = GTK_WIDGET(gtk_builder_get_object(builder, "main_box"));

//   // Initialize the menu bar and add it as the first child of main_box
//   GtkWidget *menu_bar_initialized = init_menu_bar(NULL);
//   gtk_box_pack_start(GTK_BOX(main_box), menu_bar_initialized, FALSE, FALSE, 0);

//   // Get references to other widgets
//   GtkWidget *image_widget = GTK_WIDGET(gtk_builder_get_object(builder, "image_widget"));
//   GtkWidget *hbox_buttons = GTK_WIDGET(gtk_builder_get_object(builder, "hbox_buttons"));

//   // Load a sample image
//   GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(SAMPLE_IMAGE_PATH, NULL);
//   if (pixbuf)
//   {
//     process_and_display_image(image_widget, pixbuf);
//     g_object_unref(pixbuf);
//   }
//   else
//   {
//     printf("Failed to load image from %s\n", SAMPLE_IMAGE_PATH);
//   }
//   // Add buttons to the hbox
//   const char *button_labels[] = {"Run", "Rotate left", "Rotate right"};
//   for (size_t i = 0; i < sizeof(button_labels) / sizeof(button_labels[0]); i++)
//   {
//     GtkWidget *button = init_button(button_labels[i], NULL, image_widget);
//     gtk_box_pack_start(GTK_BOX(hbox_buttons), button, FALSE, FALSE, 0);

//     // Connect the buttons to their respective callbacks
//     if (strcmp(button_labels[i], "Run") == 0)
//     {
//       g_signal_connect(button, "clicked", G_CALLBACK(on_run_clicked), image_widget);
//     }
//     else if (strcmp(button_labels[i], "Rotate left") == 0)
//     {
//       g_signal_connect(button, "clicked", G_CALLBACK(on_rotate_left_clicked), image_widget);
//     }
//     else if (strcmp(button_labels[i], "Rotate right") == 0)
//     {
//       g_signal_connect(button, "clicked", G_CALLBACK(on_rotate_right_clicked), image_widget);
//     }
//   }

//   // Show all widgets in the window
//   gtk_widget_show_all(window);
// }

int main(int argc, char *argv[])
{
  GtkBuilder *builder;
  GtkWidget *window;
  GtkWidget *image_widget;
  GdkPixbuf *pixbuf;

  gtk_init(&argc, &argv);

  // Load the UI file
  builder = gtk_builder_new_from_file("source/gtk/layout/layout.ui");

  // Get the main window
  window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));

  // Get the image widget
  image_widget = GTK_WIDGET(gtk_builder_get_object(builder, "image_widget"));

  // Check if the image widget is correctly retrieved
  if (!GTK_IS_IMAGE(image_widget))
  {
    g_print("Failed to get image widget\n");
    return 1;
  }

  // Connect signals defined in the UI file
  gtk_builder_connect_signals(builder, NULL);

  // Get the screen size
  GdkScreen *screen = gdk_screen_get_default();
  gint screen_width = gdk_screen_get_width(screen);
  gint screen_height = gdk_screen_get_height(screen);

  // Set the window size to half the screen size
  gtk_window_set_default_size(GTK_WINDOW(window), screen_width / 2, screen_height);

  // Load a sample image and set it to the image widget
  pixbuf = gdk_pixbuf_new_from_file("images/Logo_OCR_WS_fanette.png", NULL);
  if (pixbuf)
  {
    process_and_display_image(image_widget, pixbuf);
    g_object_unref(pixbuf);
  }
  else
  {
    g_print("Failed to load sample image\n");
    return 1; // Exit the program if the image fails to load
  }

  // Show the main window
  gtk_widget_show_all(window);

  // Start the GTK main loop
  gtk_main();

  return 0;
}