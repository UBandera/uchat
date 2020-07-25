#include "client.h"

#define MX_STYLES "./src/screens/main_window/dialog.css"
#define MX_MAIN_WINDOW "./src/screens/glade/main.glade"
#define MX_ALLOW_PATTERN "/^[+]{1}[0-9]{12}$"

static gboolean clear_inputs(GtkWidget *widget, GdkEventKey *event,
                             GtkEntry *search) {
    if (event->keyval == GDK_KEY_Escape) {
        t_client *client = *mx_get_client();
        GtkBuilder *builder = client->builder;
        GtkEntryBuffer* buf = gtk_entry_get_buffer(search);
        GtkWidget *info = NULL;

        info = GTK_WIDGET(gtk_builder_get_object(builder, "info_label"));
        gtk_entry_buffer_set_text(buf, "\0", -1 );
        if (client->contact_view)
            gtk_widget_hide(client->contact_view);
        gtk_widget_hide(info);
        gtk_widget_hide(widget);
        return TRUE;
    }
    return FALSE;
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
    g_free(request);
}

GtkWindow *mx_add_contact_dialog(t_client *client) {
    GtkBuilder *builder = client->builder;
    GError *error = NULL;
    GtkWindow *window = NULL;
    GtkEntry *search = NULL;

    mx_apply_styles(MX_STYLES);
    if (!gtk_builder_add_from_file(builder,
                                   MX_MAIN_WINDOW,
                                   &error))
        g_error("%s\n", error->message);
    window = GTK_WINDOW(gtk_builder_get_object(builder, "contact_dialog"));
    search = GTK_ENTRY(gtk_builder_get_object(builder, "dialog_search"));
    client->contact_view = NULL;
    gtk_entry_set_text(search, "");
    g_signal_connect(search, "activate", G_CALLBACK(find_user), client);
    gtk_window_set_decorated(window, FALSE);
    g_signal_connect(window, "key_press_event",
                     G_CALLBACK(clear_inputs), search);
    return window;
}
