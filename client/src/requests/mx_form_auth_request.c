#include "mx_client.h"

gchar *mx_form_auth_request(gchar *login, gchar *password, gint type) {
    GHashTable *data = g_hash_table_new(g_str_hash, g_str_equal);
    gchar *request = NULL;
    JsonNode *json = NULL;

    g_hash_table_insert(data, "login", login);
    g_hash_table_insert(data, "password", password);
    json = mx_init_json(type, data);
    request = json_to_string(json, FALSE);
    g_hash_table_destroy(data);
    json_node_free(json);
    return request;
}
