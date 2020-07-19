#include "client.h"

gchar *mx_form_send_message_request(gint user_id,
                                    gchar *message,
                                    gint chat_id) {
    cJSON *json = cJSON_CreateObject();
    gint64 send_time = g_get_real_time() / 1000000;
    gchar *request = NULL;

    cJSON_AddItemToObject(json,
                          "request_type",
                          cJSON_CreateNumber(2)); // temp change 
    cJSON_AddItemToObject(json, "user_id", cJSON_CreateNumber(user_id));
    cJSON_AddItemToObject(json, "chat_id", cJSON_CreateNumber(chat_id));
    cJSON_AddItemToObject(json, "send_time", cJSON_CreateNumber(send_time));
    cJSON_AddItemToObject(json, "message", cJSON_CreateString(message));
    request = cJSON_PrintUnformatted(json);
    if (!request){
        g_warning("Failed to print make request.\n");
    }
    cJSON_Delete(json);
    return request;
}
