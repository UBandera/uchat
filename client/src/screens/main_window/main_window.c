#include "client.h"

#define MX_MAIN_WINDOW "./src/screens/glade/main.glade"

static void add_contact_btn(GtkButton *button, GtkListBox *box) {
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

static void controling(GtkBuilder *builder, t_client *client) {
    GtkButton *add_contact = NULL;
    GtkSearchEntry *search = NULL;

    client->contacts_table = g_hash_table_new(NULL, NULL);
    add_contact = GTK_BUTTON(
                gtk_builder_get_object(builder, "add_contact_dialog"));

    search = GTK_SEARCH_ENTRY(gtk_builder_get_object(builder, "local_search"));
    client->contacts = GTK_LIST_BOX(gtk_builder_get_object(builder,
                                                           "contacts_box"));
    client->contact_info = GTK_WIDGET(
                         gtk_builder_get_object(client->builder,
                                                "contact_info_btn"));
    mx_chat_control(builder, client);
    g_signal_connect(search, "activate",
                     G_CALLBACK(local_search), client->contacts);
    g_signal_connect(add_contact, "clicked",
                     G_CALLBACK(add_contact_btn), client->contacts);

}

GtkWindow *mx_main_window(t_client *client) {
    GtkBuilder *builder = client->builder;
    GError *error = NULL;
    GtkWindow *window = NULL;

    // mx_apply_styles(MX_STYLES);
    if (!gtk_builder_add_from_file(builder,
                                   MX_MAIN_WINDOW,
                                   &error))
        g_error("%s\n", error->message);
    window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));
    controling(builder, client);
    return window;
}
