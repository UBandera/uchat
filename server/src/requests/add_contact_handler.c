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

gchar *mx_get_contacts_list(void) {
    gchar *response = NULL;
    cJSON *json = cJSON_CreateObject();
    cJSON *chat_list = cJSON_CreateArray();
    cJSON *contact_item = NULL;

    cJSON_AddItemToObject(json,
                          "response_type",
                          cJSON_CreateNumber(RS_CONTACT_LIST));
    cJSON_AddItemToObject(json, "chat_list", chat_list);
    contact_item = cJSON_CreateObject();
    cJSON_AddItemToArray(chat_list, contact_item);
    cJSON_AddItemToObject(contact_item, "name", cJSON_CreateString("Ivan"));
    cJSON_AddItemToObject(contact_item, "last_name", cJSON_CreateString("Ivanov"));
    cJSON_AddItemToObject(contact_item, "user_id", cJSON_CreateNumber(2));

    contact_item = cJSON_CreateObject();
    cJSON_AddItemToArray(chat_list, contact_item);
    cJSON_AddItemToObject(contact_item, "name", cJSON_CreateString("Jora"));
    cJSON_AddItemToObject(contact_item, "last_name", cJSON_CreateString("JORAH"));
    cJSON_AddItemToObject(contact_item, "user_id", cJSON_CreateNumber(3));

    contact_item = cJSON_CreateObject();
    cJSON_AddItemToArray(chat_list, contact_item);
    cJSON_AddItemToObject(contact_item, "name", cJSON_CreateString("Jo"));
    cJSON_AddItemToObject(contact_item, "last_name", cJSON_CreateString("JORAH"));
    cJSON_AddItemToObject(contact_item, "user_id", cJSON_CreateNumber(4));
    response = cJSON_PrintUnformatted(json);
    if (!response){
        g_warning("Failed to print make request.\n");
    }
    cJSON_Delete(json);
    return response;
}

void mx_add_contact_handler(cJSON *root, t_client *client) {
    // if (json_validator(root)) {
        gchar *token = cJSON_GetObjectItem(root, "token")->valuestring;
        gint user_id = cJSON_GetObjectItem(root, "user_id")->valueint;
        sqlite3 *db = *(mx_get_db());

        gchar *response = mx_get_contacts_list();
        mx_send_data(client->data_out, response);
        return;
        // }
        g_print("Not valid token\n");
    // }
    g_warning("Not valid request\n");
}
