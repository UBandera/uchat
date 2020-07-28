#include "client.h"

void mx_clear_chat(cJSON *json, t_client *client) {
    gchar *message = cJSON_GetObjectItem(json, "message")->valuestring;
    gint user_id = cJSON_GetObjectItem(json, "user_id")->valueint;
    GHashTable *contacts = client->contacts_table;
    t_contact_data *node = NULL;

    g_message("message = %s\n", message);
    node = (t_contact_data *)g_hash_table_lookup(contacts,
                                                 GINT_TO_POINTER(user_id));
    if (user_id == client->chat_with) {
        mx_remove_rows(client->chat);
    }
}
