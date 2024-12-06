#include "my_print.h"

extern GtkBuilder *builder;
GtkWidget *log_view = NULL;

void init_log_view(const char *ui_file)
{
    builder = gtk_builder_new_from_file(ui_file);
    log_view = GTK_WIDGET(gtk_builder_get_object(builder, "log_view"));
}

void my_print(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    // Print to stdout
    vprintf(format, args);

    // Determine the required buffer size
    va_list args_copy;
    va_copy(args_copy, args);
    int len = vsnprintf(NULL, 0, format, args_copy);
    va_end(args_copy);

    // Allocate buffer dynamically
    char *buffer = malloc(len + 1);
    if (buffer)
    {
        vsnprintf(buffer, len + 1, format, args);

        // Print to GtkTextView if log_view is initialized
        if (log_view)
        {
            GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(log_view));
            GtkTextIter end;
            gtk_text_buffer_get_end_iter(text_buffer, &end);
            gtk_text_buffer_insert(text_buffer, &end, buffer, -1);
        }

        free(buffer);
    }

    va_end(args);
}