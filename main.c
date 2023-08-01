#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <cmark.h>

static GtkWidget *text_view; // Widget for displaying text

static void show_markdown(GtkButton *button, gpointer user_data) {
    // Event handler for the "Open .md File" button
    // Shows a file chooser dialog, reads the content of the selected Markdown file, converts it to HTML, and displays it in the text_view

    // Create a file chooser dialog
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(gtk_file_chooser_dialog_new("Open File", GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))), GTK_FILE_CHOOSER_ACTION_OPEN, "_Cancel", GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL));
    // Get the action chosen in the file chooser dialog (open file or cancel)
    GtkFileChooserAction action = gtk_file_chooser_get_action(chooser);
    const gchar *filename;

    // Show the file chooser dialog and wait for the result
    if (gtk_dialog_run(GTK_DIALOG(chooser)) == GTK_RESPONSE_ACCEPT) {
        // If the user selected "Open", get the path to the selected file
        filename = gtk_file_chooser_get_filename(chooser);
    } else {
        // If the user selected "Cancel" or closed the dialog, close the window and exit the function
        gtk_widget_destroy(GTK_WIDGET(chooser));
        return;
    }

    // Close the file chooser dialog
    gtk_widget_destroy(GTK_WIDGET(chooser));

    // Open the file for reading
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        // If the file cannot be opened (e.g., does not exist), print an error message
        g_print("Error opening file\n");
        return;
    }

    // Determine the size of the file in bytes
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory to store the content of the file
    char *file_content = (char *)malloc(file_size + 1);
    if (file_content == NULL) {
        // If memory allocation fails, print an error message
        fclose(file);
        g_print("Memory allocation error\n");
        return;
    }

    // Read the content of the file into memory
    fread(file_content, 1, file_size, file);
    fclose(file);
    file_content[file_size] = '\0';

    // Convert the Markdown text to an HTML string using the cmark library
    char *html = cmark_markdown_to_html(file_content, file_size, 0);

    // Set the HTML content to the text_view widget
    gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view)), html, -1);

    // Free memory allocated for html and file_content
    g_free(html);
    g_free(file_content);
}

static void save_html(GtkButton *button, gpointer user_data) {
    // Event handler for the "Save HTML" button
    // Shows a file chooser dialog to save the HTML content displayed in text_view to a file

    // Create a file chooser dialog for saving a file
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(gtk_file_chooser_dialog_new("Save File", GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))), GTK_FILE_CHOOSER_ACTION_SAVE, "_Cancel", GTK_RESPONSE_CANCEL, "_Save", GTK_RESPONSE_ACCEPT, NULL));

    // Show the file chooser dialog and wait for the result
    if (gtk_dialog_run(GTK_DIALOG(chooser)) == GTK_RESPONSE_ACCEPT) {
        // If the user selected "Save", get the path for saving
        gchar *filename = gtk_file_chooser_get_filename(chooser);
        GtkTextIter start, end;
        // Get the buffer of the text_view widget
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
        // Get the start and end iterators of the text buffer
        gtk_text_buffer_get_start_iter(buffer, &start);
        gtk_text_buffer_get_end_iter(buffer, &end);
        // Get the text from the text_view widget as a string
        gchar *html = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
        // Open the file for writing
        FILE *file = fopen(filename, "w");
        if (file != NULL) {
            // Write the HTML content to the file
            fwrite(html, 1, strlen(html), file);
            fclose(file);
        }
        // Free memory allocated for filename and html
        g_free(html);
        g_free(filename);
    }
    // Close the file chooser dialog
    gtk_widget_destroy(GTK_WIDGET(chooser));
}

static void open_in_browser(GtkButton *button, gpointer user_data) {
    // Event handler for the "Open in Browser" button
    // Save the HTML content displayed in text_view to a temporary HTML file and open it in a web browser

    GtkTextIter start, end;
    // Get the buffer of the text_view widget
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    // Get the start and end iterators of the text buffer
    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_buffer_get_end_iter(buffer, &end);
    // Get the text from the text_view widget as a string
    gchar *html = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

    // Open the file "temp.html" for writing
    FILE *file = fopen("temp.html", "w");
    if (file != NULL) {
        // Write the HTML content to the file
        fwrite(html, 1, strlen(html), file);
        fclose(file);
    }

    // Free memory allocated for html
    g_free(html);

    // Open the temporary HTML file in a web browser using the system command "xdg-open" (Linux only)
    system("xdg-open temp.html");
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Declaration of variables for interface elements
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *box;
    GtkWidget *scroll;

    // Create the main window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Markdown Viewer");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    // Connect the "destroy" event of the window to the gtk_main_quit function
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a container for interface elements
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), box);

    // Create a widget for displaying text
    text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD_CHAR);
    // Create a scroll for the text widget
    scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scroll), text_view);
    gtk_box_pack_start(GTK_BOX(box), scroll, TRUE, TRUE, 0);

    // Create the "Open .md File" button
    button = gtk_button_new_with_label("Open .md File");
    gtk_box_pack_start(GTK_BOX(box), button, FALSE, FALSE, 0);
    // Connect the "clicked" event of the button to the show_markdown function
    g_signal_connect(button, "clicked", G_CALLBACK(show_markdown), NULL);

    // Create the "Save HTML" button
    button = gtk_button_new_with_label("Save HTML");
    gtk_box_pack_start(GTK_BOX(box), button, FALSE, FALSE, 0);
    // Connect the "clicked" event of the button to the save_html function
    g_signal_connect(button, "clicked", G_CALLBACK(save_html), NULL);

    // Create the "Open in Browser" button
    button = gtk_button_new_with_label("Open in Browser");
    gtk_box_pack_start(GTK_BOX(box), button, FALSE, FALSE, 0);
    // Connect the "clicked" event of the button to the open_in_browser function
    g_signal_connect(button, "clicked", G_CALLBACK(open_in_browser), NULL);

    // Show all interface elements
    gtk_widget_show_all(window);

    // Start the main GTK loop for event processing
    gtk_main();
    return 0;
}
