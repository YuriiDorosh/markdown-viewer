#include <gtk/gtk.h>


static void show_markdown(GtkButton *button, gpointer user_data) {
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(gtk_file_chooser_dialog_new("Open File", GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))), GTK_FILE_CHOOSER_ACTION_OPEN, "_Cancel", GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL));
    GtkFileChooserAction action = gtk_file_chooser_get_action(chooser);
    const gchar *filename;

    if (gtk_dialog_run(GTK_DIALOG(chooser)) == GTK_RESPONSE_ACCEPT) {
        filename = gtk_file_chooser_get_filename(chooser);
    } else {
        gtk_widget_destroy(GTK_WIDGET(chooser));
        return;
    }

    gtk_widget_destroy(GTK_WIDGET(chooser));

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        g_print("Error opening file\n");
        return;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *file_content = (char *)malloc(file_size + 1);
    if (file_content == NULL) {
        fclose(file);
        g_print("Memory allocation error\n");
        return;
    }

    fread(file_content, 1, file_size, file);
    fclose(file);
    file_content[file_size] = '\0';

    char *html = cmark_markdown_to_html(file_content, file_size, 0);

    gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view)), html, -1);
    g_free(html);
    g_free(file_content);
}

static void save_html(GtkButton *button, gpointer user_data) {
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(gtk_file_chooser_dialog_new("Save File", GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))), GTK_FILE_CHOOSER_ACTION_SAVE, "_Cancel", GTK_RESPONSE_CANCEL, "_Save", GTK_RESPONSE_ACCEPT, NULL));

    if (gtk_dialog_run(GTK_DIALOG(chooser)) == GTK_RESPONSE_ACCEPT) {
        gchar *filename = gtk_file_chooser_get_filename(chooser);
        GtkTextIter start, end;
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
        gtk_text_buffer_get_start_iter(buffer, &start);
        gtk_text_buffer_get_end_iter(buffer, &end);
        gchar *html = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
        FILE *file = fopen(filename, "w");
        if (file != NULL) {
            fwrite(html, 1, strlen(html), file);
            fclose(file);
        }
        g_free(html);
        g_free(filename);
    }
    gtk_widget_destroy(GTK_WIDGET(chooser));
}

static void open_in_browser(GtkButton *button, gpointer user_data) {
    GtkTextIter start, end;
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_buffer_get_end_iter(buffer, &end);
    gchar *html = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

    FILE *file = fopen("temp.html", "w");
    if (file != NULL) {
        fwrite(html, 1, strlen(html), file);
        fclose(file);
    }

    g_free(html);
    system("xdg-open temp.html"); // Linux command to open default web browser
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Markdown Viewer");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), box);

    GtkWidget *text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD_CHAR);

    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scroll), text_view);
    gtk_box_pack_start(GTK_BOX(box), scroll, TRUE, TRUE, 0);

    GtkWidget *button = gtk_button_new_with_label("Open .md File");
    gtk_box_pack_start(GTK_BOX(box), button, FALSE, FALSE, 0);
    g_signal_connect(button, "clicked", G_CALLBACK(show_markdown), NULL);

    GtkWidget *button_save_html = gtk_button_new_with_label("Save HTML");
    gtk_box_pack_start(GTK_BOX(box), button_save_html, FALSE, FALSE, 0);
    g_signal_connect(button_save_html, "clicked", G_CALLBACK(save_html), NULL);

    button = gtk_button_new_with_label("Open in Browser");
    gtk_box_pack_start(GTK_BOX(box), button, FALSE, FALSE, 0);
    g_signal_connect(button, "clicked", G_CALLBACK(open_in_browser), NULL);

    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}