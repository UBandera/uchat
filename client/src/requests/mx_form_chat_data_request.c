#include "mx_client.h"

gchar *mx_form_chat_data_request(gchar *user_id) {
    GHashTable *data = g_hash_table_new(g_str_hash, g_str_equal);
    gchar *request = NULL;
    JsonNode *json = NULL;

    g_hash_table_insert(data, "user_id", user_id);
    json = mx_init_json(RQ_CHAT_DATA, data);
    request = json_to_string(json, TRUE);
    g_hash_table_destroy(data);
    json_node_free(json);
    return request;
}
