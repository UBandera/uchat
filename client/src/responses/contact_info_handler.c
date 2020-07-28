#include "client.h"

// TODO: add user_id
static void delete(GtkWidget *button, gpointer user_id) {
    t_client *client = *mx_get_client();
    gchar *request = NULL;
    gint id = GPOINTER_TO_INT(user_id);

    request = mx_remove_contact_request(id, client->token);
    mx_send_data(client->data_out, request);
    gtk_widget_hide(GTK_WIDGET(client->contact_info_window));
    g_free(request);
    (void)button;
}

/*
 * PURPOSE : Autogenerates function contract comments
 *  PARAMS : json - formed json receiving from server response string
 *           client - struct that contain all nessasery data (in this case
 *           GtkBuilder for showing reaction in ui).
 * RETURNS : void
 *   NOTES : -
 */
void mx_contact_info(cJSON *json, t_client *client) {
    gint user_id = cJSON_GetObjectItem(json, "user_id")->valueint;
    const gchar *phone = cJSON_GetObjectItem(json, "phone")->valuestring;
    const gchar *first_name = cJSON_GetObjectItem(json, "first_name")->valuestring;
    const gchar *last_name = cJSON_GetObjectItem(json, "last_name")->valuestring;
    const gchar *email = cJSON_GetObjectItem(json, "email")->valuestring;

    mx_set_entry_value(client->builder, "contact_phone", phone);
    mx_set_entry_value(client->builder, "contact_first_name", first_name);
    mx_set_entry_value(client->builder, "contact_last_name", last_name);
    mx_set_entry_value(client->builder, "contact_email", email);
    gtk_widget_show(GTK_WIDGET(client->contact_info_window));
    g_signal_connect(gtk_builder_get_object(client->builder, "delete_btn"),
                     "clicked", G_CALLBACK(delete), GINT_TO_POINTER(user_id));
}
