#include "server.h"

static gboolean json_validator(cJSON *root) {
    cJSON *phone = cJSON_GetObjectItemCaseSensitive(root, "phone");
    cJSON *token = cJSON_GetObjectItemCaseSensitive(root, "token");

    if (phone && token) {
        if (cJSON_IsString(phone) && cJSON_IsString(token))
            return TRUE;
        else
            return FALSE;
    }
    else
        return FALSE;
}

void mx_get_contact_handler(cJSON *root, t_client *client) {
    // if (json_validator(root)) {
        gchar *token = cJSON_GetObjectItem(root, "token")->valuestring;
        gchar *phone = cJSON_GetObjectItem(root, "phone")->valuestring;
        sqlite3 *db = *(mx_get_db());

        // if (!g_strcmp0(client->token, token)) {
            gint user_id = mx_get_user_id_by_phone(phone, db);

            g_print("USER_ID = %d\n", user_id);
            // mx_get_profile_by_user_id(user_id, db);
            // gchar *response = mx_get_contacts_list();
            gchar *response = "{\"response_type\":3,\"user_id\":1,\"name\":\"Artem\",\"last_name\":\"Shemidko\"}";
            mx_send_data(client->data_out, response);
            // mx_send_data(client->data_out, response);
            // g_free(response);
            return;
        // }
        g_print("Not valid token\n");
    // }
    g_warning("Not valid request\n");
}
