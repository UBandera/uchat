#include "client.h"

gchar *mx_form_recovery_password_request(gchar *login) {
    cJSON *json = cJSON_CreateObject();
    gchar *request = NULL;

    cJSON_AddItemToObject(json, "request_type",
                          cJSON_CreateNumber(RQ_RECOVERY_PASSWD));
    cJSON_AddItemToObject(json, "login", cJSON_CreateString(login));
    request = cJSON_PrintUnformatted(json);
    if (!request){
        g_warning("Failed to print make request.\n");
    }
    cJSON_Delete(json);
    return request;
}
