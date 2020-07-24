#include "client.h"

/*
 * PURPOSE : Autogenerates function contract comments
 *  PARAMS : json - formed json receiving from server response string
 *           client - struct that contain all nessasery data (in this case
 *           GtkBuilder for showing reaction in ui).
 * RETURNS : void
 *   NOTES : -
 */
void mx_contact_list(cJSON *json, t_client *client) {
    cJSON *contacts = cJSON_GetObjectItemCaseSensitive(json, "chat_list");
    cJSON *data = NULL;

    cJSON_ArrayForEach(data, contacts) {
        gchar *first_name = cJSON_GetObjectItem(data, "name")->valuestring;
        gchar *last_name = cJSON_GetObjectItem(data, "last_name")->valuestring;
        gint user_id = cJSON_GetObjectItem(data, "user_id")->valueint;
        gchar *label = g_strjoin(" ", last_name, first_name, NULL);

        mx_show_contact_in_ui(client, label, user_id);
        g_free(label);

    }
}
