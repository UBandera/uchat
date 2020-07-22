#include "client.h"

static void get_contacts(t_client *client) {
    gchar *token = client->token;
    gchar *request = NULL;

    g_print("activated\n");
    request = mx_contacts_request(token);
    mx_send_data(client->data_out, request);
    g_free(request);
}

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
    gchar *token = cJSON_GetObjectItem(json, "token")->valuestring;
    GtkWindow *current = gtk_application_get_active_window(client->app);

    client->token = g_strdup(token);

    g_print("message = %s\n", message);
    mx_window_switcher(current, client->main_window); // client->phone_entering change to main window
    // get_contacts(client);
}
