#include "server.h"

gchar *mx_auth_send_response(gchar *token) {
    cJSON *json = cJSON_CreateObject();
    gchar *message = "User sign in successfully.";
    gchar *response = NULL;

    cJSON_AddItemToObject(json,
                          "response_type",
                          cJSON_CreateNumber(RS_VALID));
    cJSON_AddItemToObject(json, "token", cJSON_CreateString(token));
    cJSON_AddItemToObject(json, "message", cJSON_CreateString(message));
    response = cJSON_PrintUnformatted(json);
    if (!response){
        g_warning("Failed to print make request.\n");
    }
    cJSON_Delete(json);
    return response;
}

void mx_sign_up_request_handler(cJSON *root, t_client *client) {
    if (cJSON_GetObjectItem(root, "first_name") &&
        cJSON_GetObjectItem(root, "last_name") &&
        cJSON_GetObjectItem(root, "email")) {
        gchar *response = NULL;
        sqlite3 *db = *(mx_get_db());
        gchar *token = mx_add_user_to_bd(root, client, db);

        response = mx_auth_send_response(token);
        mx_send_data(client->data_out, response);
        g_free(response);
        return;
    }
    g_warning("Not valid request\n");
}
