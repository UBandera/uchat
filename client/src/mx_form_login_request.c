#include "mx_client.h"

void mx_form_login_request(gchar *login, gchar *password, t_client *client) {
    GHashTable *data = g_hash_table_new(g_str_hash, g_str_equal);
    gchar *request = NULL;

    g_hash_table_insert(data, g_strdup("login"), g_strdup(login));
    g_hash_table_insert(data, g_strdup("password"), g_strdup(password));

    // request = mx_json_to_str(json_parser_get_root(mx_init_json(RQ_SIGN_IN, data)));
    mx_send_data(client->data_out, request);
    g_hash_table_destroy(data);
}
