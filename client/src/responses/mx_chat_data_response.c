#include "client.h"

#define MX_CHAT_DATA_NOT_FOUND 1
#define MX_SUCCESS 0

/*
 * PURPOSE : Autogenerates function contract comments
 *  PARAMS : json - formed json receiving from server response string
 *           client - struct that contain all nessasery data (in this case
 *           GtkBuilder for showing reaction in ui in `mx_show_mesage_in_ui`
 *           function).
 * RETURNS : void
 *   NOTES : -
 */
void get_message_data_from_json(cJSON *json, t_client *client) {
    cJSON *chatting = cJSON_GetObjectItemCaseSensitive(json, "chat");
    cJSON *data = NULL;

    cJSON_ArrayForEach(data, chatting) {
        gchar *message = cJSON_GetObjectItem(data, "message")->valuestring;
        gchar *time = cJSON_GetObjectItem(data, "time")->valuestring;
        gint owner = cJSON_GetObjectItem(data, "owner")->valueint;

        // mx_show_message_in_ui(client->builder, message, time, owner);
    }
}

/*
 * PURPOSE : Autogenerates function contract comments
 *  PARAMS : json - formed json receiving from server response string
 *           client - struct that contain all nessasery data (in this case
 *           GtkBuilder for showing reaction in ui).
 * RETURNS : void
 *   NOTES : -
 */
void mx_chat_data_response(cJSON *json, t_client *client) {
    gint status = cJSON_GetObjectItem(json, "status")->valueint;
    gchar *message = cJSON_GetObjectItem(json, "message")->valuestring;

    if (status == MX_CHAT_DATA_NOT_FOUND) {
        // print label in ui with showing this error_message
        // message = "U haven't chatting with this user yet"
        g_warning("%s\n", message);
    }
    else if (status == MX_SUCCESS) {
        // load chat_history in ui
        get_message_data_from_json(json, client);
    }
    // if unknown error eccured
    else {
        // crush or printing error deppends on situation.
        g_error("Unknown error occured!");
    }
}
