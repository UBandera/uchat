#include "mx_client.h"

gchar *mx_form_contact_list_request(void) {
    gchar *request = NULL;
    JsonBuilder *builder = NULL;
    JsonNode *json = NULL;

    json = mx_init_json(RQ_CONTACT_LIST, NULL);
    request = json_to_string(json, TRUE);
    json_node_free(json);
    return request;
}
