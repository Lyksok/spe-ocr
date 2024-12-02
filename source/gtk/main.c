#include "main.h"

/**
 * @brief Callback function to destroy a widget.
 * @param widget The widget to be destroyed.
 * @return gboolean Always returns FALSE to stop the timeout.
 */
gboolean destroy_widget(gpointer widget)
{
  /**
   * TODO : this forcing method to free is not working, thus the leaks are
   * relative to the libpango as side effect of gtk
   * 1. This loop goes through each element in the `children` GList
   * 2/ Converts the child data to a GTK_WIDGET
   * 3. Ultimatly destroys the widget
   * 01/11/24
   */
  if (GTK_IS_WIDGET(widget))
  {
    GList *children, *p;
    // p pointer to current element
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

/**
 * @brief Creates a button widget.
 *
 * This function initializes and returns a new GtkButton widget.
 *
 * @param label The label for the button.
 * @param callback The callback function to be called when the button is
 * clicked.
 * @param data A gpointer to any additional data needed for the callback.
 * @return A pointer to the new GtkButton.
 */
GtkWidget *init_button(const char *label, GCallback callback, gpointer data)
{
  GtkWidget *button = gtk_button_new_with_label(label);
  if (callback != NULL)
    // connects the "clicked" signal to the callback function
    g_signal_connect(button, "clicked", callback, data);

  // Align the text to the left
  GtkWidget *txt_widget = gtk_bin_get_child(GTK_BIN(button));
  gtk_widget_set_halign(txt_widget, GTK_ALIGN_START);

  // Set the image widget as data on the button
  g_object_set_data(G_OBJECT(button), "image-widget", data);

  return button;
}
/**
 * @brief Creates the menu bar for the given GTK window.
 * This function initializes menu bar for the specified
 * GTK window. It adds the menu items to load and save image from a file
 * explorer.
 * @param image_widget The image widget to be updated when clicking on the
 * submenus.
 */
GtkWidget *init_menu_bar(GtkWidget *image_widget)
{
  GtkWidget *menu_bar, *file_menu, *file_menu_item, *load_menu_item,
      *save_menu_item, *solver_menu_item;

  // Creates a menu bar
  menu_bar = gtk_menu_bar_new();
  // Creates the menus
  file_menu = gtk_menu_new();
  file_menu_item = gtk_menu_item_new_with_label("📁 File");
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_menu_item), file_menu);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file_menu_item);

  // Creates load menu item => file_menu
  load_menu_item = gtk_menu_item_new_with_label("Load Image");
  g_signal_connect(load_menu_item, "activate", G_CALLBACK(on_change_image),
                   image_widget);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), load_menu_item);

  // Creates save menu item => file_menu
  save_menu_item = gtk_menu_item_new_with_label("Save Image");
  g_signal_connect(save_menu_item, "activate", G_CALLBACK(on_save_image),
                   image_widget);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), save_menu_item);

  solver_menu = gtk_menu_new();
  solver_menu_item = gtk_menu_item_new_with_label("🧩 Solver");
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(solver_menu_item), solver_menu);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), solver_menu_item);

  // Creates solver menu item => solver_menu
  solver_menu_item = gtk_menu_item_new_with_label("Solver");
  g_signal_connect(solver_menu_item, "activate", G_CALLBACK(create_solver_window), NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(solver_menu), solver_menu_item);

  // Grayscale processing menu
  GtkWidget *grayscale_menu = gtk_menu_new();
  GtkWidget *grayscale_menu_item = gtk_menu_item_new_with_label("🌑 Grayscale Processing");
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(grayscale_menu_item), grayscale_menu);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), grayscale_menu_item);

  GtkWidget *grayscale_item = gtk_menu_item_new_with_label("Grayscale");
  g_signal_connect(grayscale_item, "activate", G_CALLBACK(on_grayscale_clicked), image_widget);
  gtk_menu_shell_append(GTK_MENU_SHELL(grayscale_menu), grayscale_item);

  GtkWidget *contrast_item = gtk_menu_item_new_with_label("Contrast Amplification");
  g_signal_connect(contrast_item, "activate", G_CALLBACK(on_contrast_amplification_clicked), image_widget);
  gtk_menu_shell_append(GTK_MENU_SHELL(grayscale_menu), contrast_item);

  GtkWidget *median_filter_item = gtk_menu_item_new_with_label("Median Filter");
  g_signal_connect(median_filter_item, "activate", G_CALLBACK(on_median_filter_clicked), image_widget);
  gtk_menu_shell_append(GTK_MENU_SHELL(grayscale_menu), median_filter_item);

  GtkWidget *gaussian_filter_item = gtk_menu_item_new_with_label("Gaussian Filter");
  g_signal_connect(gaussian_filter_item, "activate", G_CALLBACK(on_gaussian_filter_clicked), image_widget);
  gtk_menu_shell_append(GTK_MENU_SHELL(grayscale_menu), gaussian_filter_item);

  // Thresholding menu
  GtkWidget *thresholding_menu = gtk_menu_new();
  GtkWidget *thresholding_menu_item = gtk_menu_item_new_with_label("Thresholding");
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(thresholding_menu_item), thresholding_menu);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), thresholding_menu_item);

  GtkWidget *average_item = gtk_menu_item_new_with_label("Average");
  g_signal_connect(average_item, "activate", G_CALLBACK(on_average_thresholding_clicked), image_widget);
  gtk_menu_shell_append(GTK_MENU_SHELL(thresholding_menu), average_item);

  GtkWidget *adaptive_item = gtk_menu_item_new_with_label("Adaptive (Local)");
  g_signal_connect(adaptive_item, "activate", G_CALLBACK(on_adaptive_thresholding_clicked), image_widget);
  gtk_menu_shell_append(GTK_MENU_SHELL(thresholding_menu), adaptive_item);

  GtkWidget *otsu_item = gtk_menu_item_new_with_label("Otsu (Global)");
  g_signal_connect(otsu_item, "activate", G_CALLBACK(on_otsu_thresholding_clicked), image_widget);
  gtk_menu_shell_append(GTK_MENU_SHELL(thresholding_menu), otsu_item);

  // B&W processing menu
  GtkWidget *bw_processing_menu = gtk_menu_new();
  GtkWidget *bw_processing_menu_item = gtk_menu_item_new_with_label("Grayscale Processing");
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(bw_processing_menu_item), bw_processing_menu);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), bw_processing_menu_item);

  GtkWidget *bw_median_filter_item = gtk_menu_item_new_with_label("Median Filter");
  g_signal_connect(bw_median_filter_item, "activate", G_CALLBACK(on_bw_median_filter_clicked), image_widget);
  gtk_menu_shell_append(GTK_MENU_SHELL(bw_processing_menu), bw_median_filter_item);

  GtkWidget *bw_gaussian_filter_item = gtk_menu_item_new_with_label("Gaussian Filter");
  g_signal_connect(bw_gaussian_filter_item, "activate", G_CALLBACK(on_bw_gaussian_filter_clicked), image_widget);
  gtk_menu_shell_append(GTK_MENU_SHELL(bw_processing_menu), bw_gaussian_filter_item);

  // Detection menu
  GtkWidget *detection_menu = gtk_menu_new();
  GtkWidget *detection_menu_item = gtk_menu_item_new_with_label("Detection");
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(detection_menu_item), detection_menu);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), detection_menu_item);

  GtkWidget *detect_characters_item = gtk_menu_item_new_with_label("Detect Characters");
  g_signal_connect(detect_characters_item, "activate", G_CALLBACK(on_detect_characters_clicked), image_widget);
  gtk_menu_shell_append(GTK_MENU_SHELL(detection_menu), detect_characters_item);

  GtkWidget *detect_grid_item = gtk_menu_item_new_with_label("Detect Grid");
  g_signal_connect(detect_grid_item, "activate", G_CALLBACK(on_detect_grid_clicked), image_widget);
  gtk_menu_shell_append(GTK_MENU_SHELL(detection_menu), detect_grid_item);

  GtkWidget *detect_word_list_item = gtk_menu_item_new_with_label("Detect Word List");
  g_signal_connect(detect_word_list_item, "activate", G_CALLBACK(on_detect_word_list_clicked), image_widget);
  gtk_menu_shell_append(GTK_MENU_SHELL(detection_menu), detect_word_list_item);

  GtkWidget *detect_words_item = gtk_menu_item_new_with_label("Detect Words");
  g_signal_connect(detect_words_item, "activate", G_CALLBACK(on_detect_words_clicked), image_widget);
  gtk_menu_shell_append(GTK_MENU_SHELL(detection_menu), detect_words_item);

  // Segmentation menu
  GtkWidget *segmentation_menu = gtk_menu_new();
  GtkWidget *segmentation_menu_item = gtk_menu_item_new_with_label("Segmentation");
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(segmentation_menu_item), segmentation_menu);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), segmentation_menu_item);

  GtkWidget *segment_grid_item = gtk_menu_item_new_with_label("Segment Grid");
  g_signal_connect(segment_grid_item, "activate", G_CALLBACK(on_segment_grid_clicked), image_widget);
  gtk_menu_shell_append(GTK_MENU_SHELL(segmentation_menu), segment_grid_item);

  GtkWidget *segment_words_item = gtk_menu_item_new_with_label("Segment Words");
  g_signal_connect(segment_words_item, "activate", G_CALLBACK(on_segment_words_clicked), image_widget);
  gtk_menu_shell_append(GTK_MENU_SHELL(segmentation_menu), segment_words_item);

  // Manual mode menu
  GtkWidget *manual_mode_menu = gtk_menu_new();
  GtkWidget *manual_mode_menu_item = gtk_menu_item_new_with_label("Manual Mode");
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(manual_mode_menu_item), manual_mode_menu);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), manual_mode_menu_item);

  GtkWidget *select_box_item = gtk_menu_item_new_with_label("Select Box");
  g_signal_connect(select_box_item, "activate", G_CALLBACK(on_select_box_clicked), image_widget);
  gtk_menu_shell_append(GTK_MENU_SHELL(manual_mode_menu), select_box_item);

  GtkWidget *segment_selected_item = gtk_menu_item_new_with_label("Segment Selected");
  g_signal_connect(segment_selected_item, "activate", G_CALLBACK(on_segment_selected_clicked), image_widget);
  gtk_menu_shell_append(GTK_MENU_SHELL(manual_mode_menu), segment_selected_item);

  return menu_bar;
}

/**
 * @brief Activates the app. Precisely, it lays out the widgets and connects
 * the signals.
 * @param app A pointer to the GtkApplication instance.
 */
static void activate(GtkApplication *app, gpointer user_data)
{
  GtkWidget *window;
  GtkWidget *main_box;
  GtkWidget *menu_bar;
  GtkWidget *grid;
  GtkWidget *image_container;
  GtkWidget *image_widget;
  GtkWidget *vbox_buttons;
  GtkWidget *button;

  (void)user_data; // Unused parameter removed warning
  // Create a new window
  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window),
                       "Organized Chaotic Results software");
  gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

  // Create a main box to hold the menu bar and the grid
  main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_add(GTK_CONTAINER(window), main_box);

  // Create an image widget
  image_widget = gtk_image_new();

  // Initialize the menu bar
  menu_bar = init_menu_bar(image_widget);
  gtk_box_pack_start(GTK_BOX(main_box), menu_bar, FALSE, FALSE, 0);

  // Create a grid container for the main content
  grid = gtk_grid_new();
  gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
  gtk_box_pack_start(GTK_BOX(main_box), grid, TRUE, TRUE, 0);

  // Create an image container
  image_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_halign(image_container, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(image_container, GTK_ALIGN_CENTER);
  gtk_grid_attach(GTK_GRID(grid), image_container, 0, 0, 1, 1);

  gtk_container_add(GTK_CONTAINER(image_container), image_widget);

  // Load a sample image
  GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(SAMPLE_IMAGE_PATH, NULL);
  if (pixbuf)
  {
    process_and_display_image(image_widget, pixbuf);
    g_object_unref(pixbuf);
  }
  else
  {
    printf("Failed to load image from %s\n", SAMPLE_IMAGE_PATH);
  }

  // Create a vertical box for buttons
  vbox_buttons = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_grid_attach(GTK_GRID(grid), vbox_buttons, 1, 0, 1, 1);

  // Manage entry buttons for the rotation
  // Update angle values according to user input
  GtkWidget *left_angle_entry = gtk_entry_new();
  GtkWidget *right_angle_entry = gtk_entry_new();
  char *left_angle_text = g_strdup_printf("%.2f", DEFAULT_LEFT_ANGLE);
  gtk_entry_set_text(GTK_ENTRY(left_angle_entry), left_angle_text);
  g_free(left_angle_text);

  char *right_angle_text = g_strdup_printf("%.2f", DEFAULT_RIGHT_ANGLE);
  gtk_entry_set_text(GTK_ENTRY(right_angle_entry), right_angle_text);
  g_free(right_angle_text);

  g_signal_connect(left_angle_entry, "activate",
                   G_CALLBACK(on_angle_entry_activate), &left_angle);
  // updates when the user presses enter
  g_signal_connect(right_angle_entry, "activate",
                   G_CALLBACK(on_angle_entry_activate), &right_angle);
  // updates when the user presses enter
  gtk_box_pack_start(GTK_BOX(vbox_buttons), gtk_label_new("Left angle:"), FALSE,
                     FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox_buttons), left_angle_entry, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox_buttons), gtk_label_new("Right angle:"),
                     FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox_buttons), right_angle_entry, FALSE, FALSE, 0);

  // Add buttons to the vertical box
  const char *button_labels[] = {"Grayscale", "Binarize", "Invert colors",
                                 "Rotate left", "Rotate right"};
  for (long unsigned int i = 0;
       i < sizeof(button_labels) / sizeof(button_labels[0]); i++)
  {
    button = init_button(button_labels[i], NULL, image_widget);
    gtk_box_pack_start(GTK_BOX(vbox_buttons), button, FALSE, FALSE, 0);
    // Connect the buttons to their respective effects
    if (strcmp(button_labels[i], "Grayscale") == 0)
    {
      g_signal_connect(button, "clicked", G_CALLBACK(on_grayscale_clicked),
                       image_widget);
    }
    else if (strcmp(button_labels[i], "Binarize") == 0)
    {
      g_signal_connect(button, "clicked", G_CALLBACK(on_binarize_clicked),
                       image_widget);
    }
    else if (strcmp(button_labels[i], "Rotate left") == 0)
    {
      g_signal_connect(button, "clicked", G_CALLBACK(on_rotate_left_clicked),
                       image_widget);
    }
    else if (strcmp(button_labels[i], "Rotate right") == 0)
    {
      g_signal_connect(button, "clicked", G_CALLBACK(on_rotate_right_clicked),
                       image_widget);
    }
    else if (strcmp(button_labels[i], "Invert colors") == 0)
    {
      g_signal_connect(button, "clicked", G_CALLBACK(on_invert_colors_clicked),
                       image_widget);
    }
    /**
    else if (strcmp(button_labels[i], "Detect") == 0)
    {
      g_signal_connect(button, "clicked", G_CALLBACK(on_detect_clicked),
                       image_widget);
    }
    else if (strcmp(button_labels[i], "Segment) == 0)
    {
      g_signal_connect(button, "clicked", G_CALLBACK(on_segment_clicked),
                       image_widget);
    }
    */
  }

  // Show all widgets in the window
  gtk_widget_show_all(window);

  // Show the splash screen
  show_splash_screen(app);
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
  app = gtk_application_new("org.app.OCR", G_APPLICATION_DEFAULT_FLAGS);

  /*  Signal connections */
  // "activate" signal connection
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  /*  Run event loop  */
  status = g_application_run(G_APPLICATION(app), argc, argv);

  g_object_unref(app);

  return status;
}
