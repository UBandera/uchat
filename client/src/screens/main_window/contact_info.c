#include "client.h"

#define MX_MAIN_WINDOW "./src/screens/glade/contact_info.glade"

static void controling(GtkBuilder *builder, t_client *client) {
    GtkButton *add_contact = NULL;
    GtkButton *profile = NULL;
    GtkSearchEntry *search = NULL;


    client->contacts_table = g_hash_table_new(NULL, NULL);
    client->contacts = GTK_LIST_BOX(gtk_builder_get_object(builder,
                                                           "contacts_box"));
    client->contact_info = GTK_WIDGET(
                         gtk_builder_get_object(client->builder,
                                                "contact_info_btn"));
    mx_top_bar_control(builder, client);
    mx_chat_control(builder, client);

}
// TODO: add user_id
static void delete(GtkWidget *button, GtkWidget *widget) {
    t_client *client = *mx_get_client();
    gchar *request = NULL;

    request = mx_remove_contact_request(user_id, client->token);
    mx_send_data(client->data_out, request);
    gtk_widget_hide(widget);
    g_free(request);
    (void)button;
}

static void close(GtkWidget *button, GtkWidget *widget) {
    gtk_widget_hide(widget);
    (void)button;
}

GtkWindow *mx_contact_info_window(t_client *client) {
    GError *error = NULL;
    GtkWindow *window = NULL;
    GtkWidget *del_contact = NULL;
    GtkWidget *close = NULL;

    // mx_apply_styles(MX_STYLES);
    if (!gtk_builder_add_from_file(client->builder, MX_MAIN_WINDOW, &error))
        g_error("%s\n", error->message);
    window = GTK_WINDOW(gtk_builder_get_object(client->builder, "window"));
    del_contact = GTK_WINDOW(gtk_builder_get_object(client->builder, "delete_btn"));
    close = GTK_WINDOW(gtk_builder_get_object(client->builder, "close_btn"));
    g_signal_connect(window, "clicked", G_CALLBACK(delete), client);
    g_signal_connect(window, "clicked", G_CALLBACK(close), window);
    g_signal_connect(window, "key_press_event",
                     G_CALLBACK(mx_close_widget), window);
    return window;
}
