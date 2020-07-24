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

    g_print("message = %s\n", message);
    mx_window_switcher(current, client->main_window);
}
