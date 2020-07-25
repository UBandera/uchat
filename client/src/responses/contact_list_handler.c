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

        mx_show_contact_in_ui(client, first_name, last_name, user_id);
    }
}
