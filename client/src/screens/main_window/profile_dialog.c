#include "client.h"

#define MX_MAIN_WINDOW "./src/screens/glade/profile.glade"

static void change_data(GtkWidget *widget, t_client *client) {
    GtkEntry *phone = GTK_ENTRY(gtk_builder_get_object(client->builder,
                                                       "profile_phone"));
    GtkEntry *first_name = GTK_ENTRY(gtk_builder_get_object(client->builder,
                                                            "profile_first_name"));
    GtkEntry *last_name = GTK_ENTRY(gtk_builder_get_object(client->builder,
                                                           "profile_last_name"));
    GtkEntry *email = GTK_ENTRY(gtk_builder_get_object(client->builder,
                                                       "profile_email"));
    gchar *request = NULL;

    request = mx_change_data_request(gtk_entry_get_text(phone),
                                      gtk_entry_get_text(first_name),
                                      gtk_entry_get_text(last_name),
                                      gtk_entry_get_text(email),
                                      client->token);
    mx_send_data(client->data_out, request);
    g_free(request);
}


GtkWindow *mx_profile_window(t_client *client) {
    GtkBuilder *builder = client->builder;
    GError *error = NULL;
    GtkWindow *window = NULL;
    GtkWidget *change = NULL;

    // mx_apply_styles(MX_STYLES);
    if (!gtk_builder_add_from_file(builder, MX_MAIN_WINDOW, &error))
        g_error("%s\n", error->message);
    window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));
    change = GTK_WIDGET(gtk_builder_get_object(builder, "change_btn"));
    g_signal_connect(change, "clicked", G_CALLBACK(change_data), client);
    gtk_window_set_transient_for(window, client->main_window);
    g_signal_connect(window, "key_press_event",
                     G_CALLBACK(mx_close_widget), window);
    return window;
}
