#include "client.h"

gchar *mx_add_contact_request(gchar *phone, gchar *token) {
    cJSON *json = cJSON_CreateObject();
    gchar *request = NULL;

    cJSON_AddItemToObject(json, "request_type", cJSON_CreateNumber(RQ_GET_USER));
    cJSON_AddItemToObject(json, "token", cJSON_CreateString(token));
    cJSON_AddItemToObject(json, "phone", cJSON_CreateString(phone));
    request = cJSON_PrintUnformatted(json);
    if (!request){
        g_warning("Failed to print make request.\n");
    }
    cJSON_Delete(json);
    return request;
}
