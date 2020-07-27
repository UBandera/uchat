#include "client.h"

static void add_contact_btn_callback(GtkButton *button, GtkListBox *box) {
    t_client *client = *mx_get_client();
    GtkWindow *current = gtk_application_get_active_window(client->app);

    gtk_widget_show(GTK_WIDGET(client->add_contact_dialog));
}

static gboolean searching(gchar *label, gchar *search) {
    for (int i = 0; label[i] && search[i]; i++) {
        if (label[i] != search[i])
            return FALSE;
    }
    return TRUE;
}

static void local_search(GtkEntry *entry, GtkListBox *box) {
    gchar *search = (gchar *)gtk_entry_get_text(entry);
    GtkListBoxRow *row = NULL;

    for (int i = 0; ; i++) {
        if (!(row = gtk_list_box_get_row_at_index(box, i))) {
            break;
        }
        GtkButton *child = GTK_BUTTON(gtk_bin_get_child(GTK_BIN(row)));
        gchar *label = (gchar *)gtk_button_get_label(child);
        if (!searching(label, search))
            gtk_widget_hide(GTK_WIDGET(row));
        else
            gtk_widget_show(GTK_WIDGET(row));
    }
}

static void log_out(GtkButton *button, gpointer data) {
    t_client *client = *mx_get_client();
    gchar *request = NULL;

    request = mx_sign_out_request(client->token);
    mx_send_data(client->data_out, request);
    g_free(request);
}

void mx_top_bar_control(GtkBuilder *builder, t_client *client) {
    GtkButton *add_contact = NULL;
    GtkButton *profile = NULL;
    GtkSearchEntry *search = NULL;

    profile = GTK_BUTTON(gtk_builder_get_object(builder, "props"));
    add_contact = GTK_BUTTON(
                gtk_builder_get_object(builder, "add_contact_dialog"));

    search = GTK_SEARCH_ENTRY(gtk_builder_get_object(builder, "local_search"));

    g_signal_connect(search, "activate",
                     G_CALLBACK(local_search), client->contacts);
    g_signal_connect(add_contact, "clicked",
                     G_CALLBACK(add_contact_btn_callback), client->contacts);
    g_signal_connect(profile, "clicked", G_CALLBACK(log_out), NULL);
}
