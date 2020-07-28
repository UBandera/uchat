#include "client.h"

void mx_remove_contact(cJSON *json, t_client *client) {
    gchar *message = cJSON_GetObjectItem(json, "message")->valuestring;
    gint user_id = cJSON_GetObjectItem(json, "contact")->valueint;
    GHashTable *contacts = client->contacts_table;
    t_contact_data *node = NULL;

    node = (t_contact_data *)g_hash_table_lookup(contacts,
                                                 GINT_TO_POINTER(user_id));
    gtk_widget_destroy(GTK_WIDGET(node->row));
    g_free(node->first_name);
    g_free(node->last_name);
    mx_remove_rows(GTK_LIST_BOX(client->chat));
    g_hash_table_remove(contacts, GINT_TO_POINTER(user_id));
    client->chat_with = 0;
}
