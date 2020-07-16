#include "client.h"

gchar *mx_form_send_message_request(gint user_id, gchar *message) {
    cJSON *json = cJSON_CreateObject();
    gchar *request = NULL;

    cJSON_AddItemToObject(json,
                          "request_type",
                          cJSON_CreateNumber(RQ_SEND_MESSAGE));
    cJSON_AddItemToObject(json, "user_id", cJSON_CreateNumber(user_id));
    cJSON_AddItemToObject(json, "message", cJSON_CreateString(message));
    request = cJSON_PrintUnformatted(json);
    if (!request){
        g_warning("Failed to print make request.\n");
    }
    cJSON_Delete(json);
    return request;
}
