#include "mx_client.h"

gchar *mx_form_send_message_request(gchar *user_id, gchar *message) {
    GHashTable *data = g_hash_table_new(g_str_hash, g_str_equal);
    gchar *request = NULL;
    JsonNode *json = NULL;

    g_hash_table_insert(data, "user_id", user_id);
    g_hash_table_insert(data, "message", message);
    json = mx_init_json(RQ_SEND_MESSAGE, data);
    request = json_to_string(json, TRUE);
    g_hash_table_destroy(data);
    json_node_free(json);
    return request;
}
