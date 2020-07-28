#include "client.h"

#define MX_STYLES "./src/screens/auth/test.css"

/*
 * PURPOSE : Autogenerates function contract comments
 *  PARAMS : json - formed json receiving from server response string
 *           client - struct that contain all nessasery data (in this case
 *           GtkBuilder for showing reaction in ui).
 * RETURNS : void
 *   NOTES : -
 */
void mx_get_message_handler(cJSON *json, t_client *client) {
    GtkBuilder *builder = client->builder;
    GtkTextBuffer *buffer = NULL;
    gint contact = cJSON_GetObjectItem(json, "sender_id")->valueint;

    if (client->chat_with == contact) {
        gchar *request = mx_chat_history_request(contact, client->token, 0, 1);

        mx_send_data(client->data_out, request);
        g_free(request);
    }
    else {
        GHashTable *contacts = client->contacts_table;
        t_contact_data *node = NULL;
        node = (t_contact_data *)g_hash_table_lookup(contacts,
                GINT_TO_POINTER(contact));
        GtkWidget *child = GTK_WIDGET(gtk_bin_get_child(GTK_BIN(node->row)));
        gtk_widget_set_name(child, "notify_available");
        mx_apply_styles(MX_STYLES);
    }
    // buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(client->text_view));
    // message = mx_get_message_from_entry(client->text_view);
    // mx_show_message_in_ui(client, message);
    // gtk_text_buffer_set_text(buffer, "\0", -1);
    // g_free(message);
}
