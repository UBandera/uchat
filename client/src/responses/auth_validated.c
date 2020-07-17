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
    // gchar *token = cJSON_GetObjectItem(json, "token")->valuestring;

    g_print("message is %s\n", message);
    // client->token = token;
    mx_window_switcher(client->profile_setuping, client->phone_entering); // client->phone_entering change to main window
}
