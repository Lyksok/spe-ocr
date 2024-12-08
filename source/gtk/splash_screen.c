#include "splash_screen.h"

/**
 * @brief Shows the splash screen.
 * @param app The GtkApplication instance.
 * @note The splash screen is destroyed after a timeout.
 */
void show_splash_screen(GtkApplication *app) {
  printf("🖼️ Showing splash screen\n");
  GtkWidget *splash_window, *splash_img;

  // Create splash screen window
  splash_window = gtk_application_window_new(app);
  gtk_window_set_position(GTK_WINDOW(splash_window), GTK_WIN_POS_CENTER);
  gtk_widget_set_size_request(splash_window, SPLASH_MAX_WIDTH,
                              SPLASH_MAX_HEIGHT); // standard wizard size
  gtk_window_set_decorated(
      GTK_WINDOW(splash_window),
      FALSE); // remove default title bar, resize controls, etc.
  my_print("🪟 Splash screen window created\n");

  // Load splash screen image
  splash_img = gtk_image_new_from_file(SPLASH_IMAGE_PATH);
  gtk_container_add(GTK_CONTAINER(splash_window), splash_img);
  printf("🖼️ Splash screen image loaded\n");

  gtk_window_set_keep_above(
      GTK_WINDOW(splash_window),
      TRUE); // Keep splash screen on top of every other window
  printf("🔝 Splash screen set to stay on top\n");

  // Show splash screen
  gtk_widget_show_all(splash_window);
  my_print("✅ Splash screen shown\n");

  // Splash screen exits with timeout
  g_timeout_add(3000, destroy_widget, splash_window);
  printf("⏲️ Splash screen will be destroyed after timeout\n");
}