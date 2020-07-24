#include "client.h"
#define MX_MAIN_WINDOW "./src/screens/glade/main.glade"
#define MX_ALLOW_PATTERN "/^[+]{1}[0-9]{12}$"



static void add_contact(GtkButton *button, t_client *client) {
    GtkBuilder *builder = client->builder;
    GtkEntry *search = NULL;
    gchar *request = NULL;
    gchar *contact = NULL;

    search = GTK_ENTRY(gtk_builder_get_object(builder, "dialog_search"));
    contact = (gchar *)gtk_entry_get_text(search);
    // request = mx_add_contact_request(contact, contact_id);
    // mx_send_data(client->data_out, request);
    gtk_widget_hide(GTK_WIDGET(client->add_contact_dialog));
    (void)button;
}

static void find_user(GtkEntry *entry, t_client *client) {
    GtkBuilder *builder = client->builder;
    GtkLabel *info = GTK_LABEL(gtk_builder_get_object(builder, "info_label"));
    const gchar *input = gtk_entry_get_text(entry);
    gchar *request = NULL;

    gtk_widget_hide(GTK_WIDGET(info));
    if (strlen(input) != 13) {
        gtk_label_set_text(info, "Enter full number");
        gtk_widget_show(GTK_WIDGET(info));
        return;
    }
    if (mx_match(input, MX_ALLOW_PATTERN, 0, 0)) {
        gtk_label_set_text(info, "Not valid phone number");
        gtk_widget_show(GTK_WIDGET(info));
        return;
    }
    request = mx_find_contact_request((gchar *)input, "client->token");
    mx_send_data(client->data_out, request);
}

static void controling(GtkBuilder *builder, t_client *client) {
    GtkButton *add_btn = NULL;
    GtkEntry *search = NULL;

    add_btn = GTK_BUTTON(gtk_builder_get_object(builder, "add_contact_btn"));
    search = GTK_ENTRY(gtk_builder_get_object(builder, "dialog_search"));
    g_signal_connect(search, "activate", G_CALLBACK(find_user), client);
    g_signal_connect(add_btn, "clicked", G_CALLBACK(add_contact), client);
}

GtkWindow *mx_add_contact_dialog(t_client *client) {
    GtkBuilder *builder = client->builder;
    GError *error = NULL;
    GtkWindow *window = NULL;

    // mx_apply_styles(MX_STYLES);
    if (!gtk_builder_add_from_file(builder,
                                   MX_MAIN_WINDOW,
                                   &error))
        g_error("%s\n", error->message);
    window = GTK_WINDOW(gtk_builder_get_object(builder, "contact_dialog"));
    gtk_window_set_decorated(window, FALSE);
    controling(builder, client);
    g_signal_connect(window, "key_press_event",
                     G_CALLBACK(mx_close_window_by_esc), window);
    return window;
}
