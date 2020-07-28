#include "client.h"

/*
 * PURPOSE : Autogenerates function contract comments
 *  PARAMS : json - formed json receiving from server response string
 *           client - struct that contain all nessasery data (in this case
 *           GtkBuilder for showing reaction in ui).
 * RETURNS : void
 *   NOTES : -
 */
void mx_auth_validated(cJSON *json, t_client *client) {
    gchar *message = cJSON_GetObjectItem(json, "message")->valuestring;
    GtkWindow *current = gtk_application_get_active_window(client->app);
    gchar *request = NULL;

    g_print("message = %s\n", message);
    if (current == client->profile_setuping) {
        mx_set_entry_value(client->builder, "first_name", "\0");
        mx_set_entry_value(client->builder, "last_name", "\0");
        mx_set_entry_value(client->builder, "email", "\0");
    }
    mx_window_switcher(current, client->main_window);
    request = mx_contact_list_request(client->token);
    mx_send_data(client->data_out, request);
    g_free(request);
}
