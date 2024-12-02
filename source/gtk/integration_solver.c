#include <gtk/gtk.h>
#include "../solver/solver.h"
#include "../solver/read.h"

// Callback function to handle the solver
void on_solve_button_clicked(GtkWidget *widget, gpointer data)
{
    GtkWidget **widgets = (GtkWidget **)data;
    GtkEntry *grid_entry = GTK_ENTRY(widgets[0]);
    GtkEntry *word_entry = GTK_ENTRY(widgets[1]);
    GtkTextView *text_view = GTK_TEXT_VIEW(widgets[2]);

    const char *grid_text = gtk_entry_get_text(grid_entry);
    const char *word = gtk_entry_get_text(word_entry);

    if (grid_text == NULL || strlen(grid_text) == 0 || word == NULL || strlen(word) == 0)
    {
        return;
    }

    // Parse the grid text into a 2D array
    int row = 8;  // Example row count
    int col = 10; // Example column count
    char **grid = ReadFile(grid_text, &row, &col);

    if (grid == NULL)
    {
        gtk_text_buffer_set_text(gtk_text_view_get_buffer(text_view), "Invalid grid input", -1);
        return;
    }

    // Call the solver function
    int found = Solver(row, col, grid, word);

    // Display the results in the text view
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(text_view);
    gtk_text_buffer_set_text(buffer, found ? "Word found !" : "Word not found !", -1);

    // Free the grid memory
    FreeMat(grid, row);
}

void create_solver_window(GtkWidget *widget, gpointer data)
{
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *grid_entry;
    GtkWidget *word_entry;
    GtkWidget *solve_button;
    GtkWidget *text_view;
    GtkWidget *widgets[3];

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Solver");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    grid_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(grid_entry), "Enter grid as text");
    gtk_grid_attach(GTK_GRID(grid), grid_entry, 0, 0, 2, 1);

    word_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(word_entry), "Enter word to search");
    gtk_grid_attach(GTK_GRID(grid), word_entry, 0, 1, 2, 1);

    solve_button = gtk_button_new_with_label("Solve");
    gtk_grid_attach(GTK_GRID(grid), solve_button, 0, 2, 2, 1);

    text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_grid_attach(GTK_GRID(grid), text_view, 0, 3, 2, 1);

    widgets[0] = grid_entry;
    widgets[1] = word_entry;
    widgets[2] = text_view;
    g_signal_connect(solve_button, "clicked", G_CALLBACK(on_solve_button_clicked), widgets);

    gtk_widget_show_all(window);
}