#include "mx_client.h"

gchar *mx_form_profile_data_request(void) {
    gchar *request = NULL;
    JsonNode *json = NULL;

    json = mx_init_json(RQ_PROFILE_DATA, NULL);
    request = json_to_string(json, TRUE);
    json_node_free(json);
    return request;
}
