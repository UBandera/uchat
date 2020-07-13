#include "client.h"

gchar *mx_form_chat_data_request(gint user_id) {
    cJSON *json = cJSON_CreateObject();
    gchar *request = NULL;

    cJSON_AddItemToObject(json,
                          "request_type",
                          cJSON_CreateNumber(RQ_CHAT_DATA));
    cJSON_AddItemToObject(json, "user_id", cJSON_CreateNumber(user_id));
    request = cJSON_PrintUnformatted(json);
    if (!request){
        g_warning("Failed to print make request.\n");
    }
    cJSON_Delete(json);
    return request;
}
