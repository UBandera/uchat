#include "client.h"

gchar *mx_set_up_profile_request(const gchar *first_name,
                                 const gchar *last_name,
                                 const gchar *email) {
    gint type = RQ_SET_UP_PROFILE;
    cJSON *json = cJSON_CreateObject();
    gchar *request = NULL;

    cJSON_AddItemToObject(json, "request_type", cJSON_CreateNumber(type));
    cJSON_AddItemToObject(json, "first_name", cJSON_CreateString(first_name));
    cJSON_AddItemToObject(json, "last_name", cJSON_CreateString(last_name));
    cJSON_AddItemToObject(json, "email", cJSON_CreateString(email));
    request = cJSON_PrintUnformatted(json);
    if (!request){
        g_warning("Failed to print make request.\n");
    }
    cJSON_Delete(json);
    return request;
}
