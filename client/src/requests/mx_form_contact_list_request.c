#include "client.h"

gchar *mx_form_contact_list_request(void) {
    cJSON *json = cJSON_CreateObject();
    gchar *request = NULL;

    cJSON_AddItemToObject(json,
                          "request_type",
                          cJSON_CreateNumber(RQ_CONTACT_LIST));
    request = cJSON_Print(json);
    if (!request){
        g_warning("Failed to print make request.\n");
    }
    cJSON_Delete(json);
    return request;
}
