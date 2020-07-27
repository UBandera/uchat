#include "client.h"

void mx_send_message_handler(cJSON *json, t_client *client) {
    GtkBuilder *builder = client->builder;
    gchar *message = NULL;
    GtkTextBuffer *buffer = NULL;

    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(client->text_view));
    message = mx_get_message_from_entry(client->text_view);
    mx_show_message_in_ui(client, message);
    gtk_text_buffer_set_text(buffer, "\0", -1);
    g_free(message);
}