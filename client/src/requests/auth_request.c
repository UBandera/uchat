#include "client.h"

gchar *mx_auth_request(const gchar *phone, const gchar *password) {
    cJSON *json = cJSON_CreateObject();
    gchar *request = NULL;
    gint type = RQ_AUTH;

    cJSON_AddItemToObject(json, "request_type", cJSON_CreateNumber(type));
    cJSON_AddItemToObject(json, "phone", cJSON_CreateString(phone));
    cJSON_AddItemToObject(json, "password", cJSON_CreateString(password));
    request = cJSON_PrintUnformatted(json);
    if (!request){
        g_warning("Failed to print make request.\n");
    }
    cJSON_Delete(json);
    return request;
}
