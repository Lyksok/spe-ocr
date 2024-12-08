#include "integration_documentation.h"
#include <gtk/gtk.h>
#include <gtksourceview/gtksource.h>
#include <stdio.h>
#include <stdlib.h>

void on_documentation_clicked(GtkWidget *widget, gpointer data) {
  (void)widget; // Remove unused parameter warning
  (void)data;   // Remove unused parameter warning

  my_print("📚 Documentation button clicked\n");

  GtkWidget *window;
  GtkWidget *scrolled_window;
  GtkWidget *source_view;
  GtkSourceBuffer *buffer;

  // Create a new window
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Documentation");
  gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
  printf("🪟 Documentation window created\n");

  // Connect the delete event to destroy the window
  g_signal_connect(window, "delete-event", G_CALLBACK(gtk_widget_destroy),
                   NULL);
  printf("🔗 Connected delete-event signal to documentation window\n");

  // Create a scrolled window
  scrolled_window = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_add(GTK_CONTAINER(window), scrolled_window);
  printf("📜 Scrolled window created and added to documentation window\n");

  // Create a source view
  source_view = gtk_source_view_new();
  gtk_source_view_set_show_line_numbers(GTK_SOURCE_VIEW(source_view), TRUE);
  gtk_container_add(GTK_CONTAINER(scrolled_window), source_view);
  printf("📝 Source view created and added to scrolled window\n");

  // Load the README.md file
  FILE *file = fopen("README.md", "r");
  if (file) {
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *content = malloc(length + 1);
    fread(content, 1, length, file);
    content[length] = '\0';
    fclose(file);
    my_print("📄 README.md file loaded\n");

    // Set the text buffer with the content of README.md
    buffer = gtk_source_buffer_new(NULL);
    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(buffer), content, -1);
    gtk_text_view_set_buffer(GTK_TEXT_VIEW(source_view),
                             GTK_TEXT_BUFFER(buffer));
    free(content);
    printf("📝 Text buffer set with README.md content\n");

    // Apply Markdown syntax highlighting
    GtkSourceLanguageManager *lm = gtk_source_language_manager_get_default();
    GtkSourceLanguage *lang =
        gtk_source_language_manager_get_language(lm, "markdown");
    gtk_source_buffer_set_language(buffer, lang);
    printf("🌈 Markdown syntax highlighting applied\n");
    // TODO not working!!
  } else {
    buffer = gtk_source_buffer_new(NULL);
    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(buffer),
                             "Failed to load README.md", -1);
    gtk_text_view_set_buffer(GTK_TEXT_VIEW(source_view),
                             GTK_TEXT_BUFFER(buffer));
    my_print("❌ Failed to load README.md file\n");
  }

  // Show all widgets in the window
  gtk_widget_show_all(window);
  my_print("✅ Documentation window shown\n");
}