#include "client.h"

gchar *mx_contacts_request(const gchar *token) {
    cJSON *json = cJSON_CreateObject();
    gchar *request = NULL;
    gint type = RQ_CONTACTS_LIST;

    cJSON_AddItemToObject(json, "request_type", cJSON_CreateNumber(type));
    cJSON_AddItemToObject(json, "token", cJSON_CreateString(token));
    request = cJSON_PrintUnformatted(json);
    if (!request){
        g_warning("Failed to print make request.\n");
    }
    cJSON_Delete(json);
    return request;
}
