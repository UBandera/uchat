#include "client.h"

gchar *mx_form_send_message_request(gint receiver_id,
                                    gchar *message) {
    cJSON *json = cJSON_CreateObject();
    gchar *request = NULL;

    cJSON_AddItemToObject(json,
                          "request_type",
                          cJSON_CreateNumber(2)); // temp change 
    cJSON_AddItemToObject(json, "receiver_id", cJSON_CreateNumber(receiver_id));
    cJSON_AddItemToObject(json, "message", cJSON_CreateString(message));
    request = cJSON_PrintUnformatted(json);
    if (!request){
        g_warning("Failed to print make request.\n");
    }
    cJSON_Delete(json);
    return request;
}
