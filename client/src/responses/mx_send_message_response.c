#include "client.h"

#define MX_SUCCESS 0

/*
 * PURPOSE : Autogenerates function contract comments
 *  PARAMS : json - formed json receiving from server response string
 *           client - struct that contain all nessasery data (in this case
 *           GtkBuilder for showing reaction in ui).
 * RETURNS : void
 *   NOTES : -
 */
void mx_send_message_response(cJSON *json, t_client *client) {
    gint status = cJSON_GetObjectItem(json, "status")->valueint;

    if (status == MX_SUCCESS) {
        // load message in chat ui
        mx_chat_data_response(json, client);
    }
    // if unknown error eccured
    else {
        // crush or printing error deppends on situation.
        g_error("Unknown error occured!");
    }
}
