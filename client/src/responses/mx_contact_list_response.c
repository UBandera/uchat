#include "client.h"

#define MX_CONTACTS_NOT_FOUND 1
#define MX_SUCCESS 0

/*
 * PURPOSE : Autogenerates function contract comments
 *  PARAMS : json - formed json receiving from server response string
 *           client - struct that contain all nessasery data (in this case
 *           GtkBuilder for showing reaction in ui in `mx_show_contact_in_ui`
 *           function).
 * RETURNS : void
 *   NOTES : -
 */
static void get_contact_from_json(cJSON *json, t_client *client) {
    cJSON *contacts = cJSON_GetObjectItemCaseSensitive(json, "contacts");
    cJSON *data = NULL;

    cJSON_ArrayForEach(data, contacts) {
        gchar *username = cJSON_GetObjectItem(data, "username")->valuestring;
        gint user_id = cJSON_GetObjectItem(data, "user_id")->valueint;

        // mx_show_contact_in_ui(client->builder, username, user_id);
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
void mx_contact_list_response(cJSON *json, t_client *client) {
    gint status = cJSON_GetObjectItem(json, "status")->valueint;
    gchar *message = cJSON_GetObjectItem(json, "message")->valuestring;

    if (status == MX_CONTACTS_NOT_FOUND) {
        // print label in ui with showing this error_message
        // message = "U haven't contacts in list yet. Use search to find new."
        g_warning("%s\n", message);
    }
    else if (status == MX_SUCCESS) {
        // show grid with contacts in ui
        get_contact_from_json(json, client);
    }
    // if unknown error eccured
    else {
        // crush or printing error deppends on situation.
        g_error("Unknown error occured!");
    }
}
