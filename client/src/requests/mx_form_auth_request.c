#include "mx_client.h"

gchar *mx_form_auth_request(gchar *login, gchar *password, gint type) {
    cJSON *json = cJSON_CreateObject();
    gchar *request = NULL;

    cJSON_AddItemToObject(json, "request_type", cJSON_CreateNumber(type));
    cJSON_AddItemToObject(json, "login", cJSON_CreateString(login));
    cJSON_AddItemToObject(json, "password", cJSON_CreateString(password));
    request = cJSON_Print(json);
    if (!request){
        g_warning("Failed to print make request.\n");
    }
    cJSON_Delete(json);
    return request;
}
