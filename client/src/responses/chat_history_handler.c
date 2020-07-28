#include "client.h"

/*
 * PURPOSE : Autogenerates function contract comments
 *  PARAMS : json - formed json receiving from server response string
 *           client - struct that contain all nessasery data (in this case
 *           GtkBuilder for showing reaction in ui).
 * RETURNS : void
 *   NOTES : -
 */
void mx_get_chat_history(cJSON *json, t_client *client) {
    cJSON *contacts = cJSON_GetObjectItemCaseSensitive(json, "messages");
    cJSON *data = NULL;

    cJSON_ArrayForEach(data, contacts) {
        gchar *message = cJSON_GetObjectItem(data, "message")->valuestring;
        // gint date = cJSON_GetObjectItem(data, "date")->valueint;

        g_print("chat history handler\n");
        mx_show_message_in_ui(client, message);
    }
}
