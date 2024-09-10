#include <stdio.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp> // Assuming OpenCV is used for image processing
#include <gtk/gtk.h> // Assuming GTK is used for GUI

// Structure for the crossword solver application
typedef struct {
    char *image_path;
    cv::Mat original_image;
    cv::Mat grayscale_image;
    GtkWidget *window;
    GtkWidget *image_widget;
} CrosswordSolver;

// Function prototypes
void loadImage(CrosswordSolver *solver, const char *path);
void convertToGrayscale(CrosswordSolver *solver);
void createGUI(CrosswordSolver *solver);
void onOpenImage(GtkWidget *widget, gpointer data);

int main(int argc, char *argv[]) {
    CrosswordSolver solver = {0};

    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create GUI
    createGUI(&solver);

    // Start GTK main loop
    gtk_main();

    return 0;
}

// Function to load an image
void loadImage(CrosswordSolver *solver, const char *path) {
    solver->original_image = cv::imread(path, cv::IMREAD_COLOR);
    if (solver->original_image.empty()) {
        fprintf(stderr, "Error loading image\n");
        return;
    }
    solver->image_path = strdup(path);
}

// Function to convert image to grayscale
void convertToGrayscale(CrosswordSolver *solver) {
    cv::cvtColor(solver->original_image, solver->grayscale_image, cv::COLOR_BGR2GRAY);
}

// Function to create the GUI
void createGUI(CrosswordSolver *solver) {
    solver->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(solver->window), "Crossword Solver");
    gtk_window_set_default_size(GTK_WINDOW(solver->window), 800, 600);

    GtkWidget *vbox = gtk_vbox_new(FALSE, 5);
    gtk_container_add(GTK_CONTAINER(solver->window), vbox);

    GtkWidget *open_button = gtk_button_new_with_label("Open Image");
    g_signal_connect(open_button, "clicked", G_CALLBACK(onOpenImage), solver);
    gtk_box_pack_start(GTK_BOX(vbox), open_button, FALSE, FALSE, 0);

    solver->image_widget = gtk_image_new();
    gtk_box_pack_start(GTK_BOX(vbox), solver->image_widget, TRUE, TRUE, 0);

    g_signal_connect(solver->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(solver->window);
}

// Callback function for opening an image
void onOpenImage(GtkWidget *widget, gpointer data) {
    CrosswordSolver *solver = (CrosswordSolver *)data;

    GtkWidget *dialog = gtk_file_chooser_dialog_new("Open Image",
                                                    GTK_WINDOW(solver->window),
                                                    GTK_FILE_CHOOSER_ACTION_OPEN,
                                                    "_Cancel", GTK_RESPONSE_CANCEL,
                                                    "_Open", GTK_RESPONSE_ACCEPT,
                                                    NULL);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        loadImage(solver, filename);
        convertToGrayscale(solver);

        // Display the image in the GUI
        GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
        gtk_image_set_from_pixbuf(GTK_IMAGE(solver->image_widget), pixbuf);
        g_object_unref(pixbuf);
        g_free(filename);
    }

    gtk_widget_destroy(dialog);
}