#include "server.h"

static gint profile_setup_query(sqlite3 *db, gchar *name,
                                gchar *last_name, gchar *email) {
    gchar *query = "INSERT INTO user_profile(user_id, name, last_name, email) \
                   VALUES(?, ?, ?, ?);";
    sqlite3_stmt *stmt = NULL;
    gint rc = 0;

    if ((rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0)) != SQLITE_OK)
        g_warning("profile_setup prepare: %s\n", sqlite3_errmsg(db));
    if ((sqlite3_bind_text(stmt, 1, "1", -1, NULL)) != SQLITE_OK)
        g_warning("profile_setup bind: %s\n", sqlite3_errmsg(db));
    if ((sqlite3_bind_text(stmt, 2, name, -1, NULL)) != SQLITE_OK)
        g_warning("profile_setup bind: %s\n", sqlite3_errmsg(db));
    if ((sqlite3_bind_text(stmt, 3, last_name, -1, NULL)) != SQLITE_OK)
        g_warning("profile_setup bind: %s\n", sqlite3_errmsg(db));
    if ((sqlite3_bind_text(stmt, 4, email, -1, NULL)) != SQLITE_OK)
        g_warning("profile_setup bind: %s\n", sqlite3_errmsg(db));
    if ((rc = sqlite3_step(stmt)) != SQLITE_DONE)
        g_warning("profile_setup bind: %s\n", sqlite3_errmsg(db));
    if ((rc = sqlite3_finalize(stmt)) != SQLITE_OK)
        g_warning("profile_setup finalize: %s\n", sqlite3_errmsg(db));
    return rc;
}

static gchar *auth_send_response(void) {
    cJSON *json = cJSON_CreateObject();
    gchar *message = "User sign in successfully.";
    gchar *response = NULL;

    cJSON_AddItemToObject(json,
                          "response_type",
                          cJSON_CreateNumber(RS_VALID));
    cJSON_AddItemToObject(json, "message", cJSON_CreateString(message));
    response = cJSON_PrintUnformatted(json);
    if (!response){
        g_warning("Failed to print make request.\n");
    }
    cJSON_Delete(json);
    return response;
}

static gchar *user_not_exist_response(void) {
    cJSON *json = cJSON_CreateObject();
    gchar *message = "User not exist";
    gchar *response = NULL;

    cJSON_AddItemToObject(json,
                          "response_type",
                          cJSON_CreateNumber(RS_SIGN_UP));
    cJSON_AddItemToObject(json, "message", cJSON_CreateString(message));
    response = cJSON_PrintUnformatted(json);
    if (!response){
        g_warning("Failed to print make request.\n");
    }
    cJSON_Delete(json);
    return response;
}

static gchar *error_send_response(void) {
    cJSON *json = cJSON_CreateObject();
    gchar *message = "Invalid password";
    gchar *error_res = NULL;

    cJSON_AddItemToObject(json,
                          "error_type",
                          cJSON_CreateNumber(ER_PASS));
    cJSON_AddItemToObject(json, "message", cJSON_CreateString(message));
    error_res = cJSON_PrintUnformatted(json);
    if (!error_res){
        g_warning("Failed to print make request.\n");
    }
    cJSON_Delete(json);
    return error_res;
}

static void send_response(int status, t_client *client) {
    gchar *response = NULL;

    if (!status)
        response = error_send_response();
    else if (status == 1)
        response = auth_send_response();
    else
        response = user_not_exist_response();
    mx_send_data(client->data_out, response);
    g_free(response);
}

void mx_sign_up_request_handler(cJSON *root, t_client *client) {
    if (cJSON_GetObjectItem(root, "first_name") &&
        cJSON_GetObjectItem(root, "last_name") &&
        cJSON_GetObjectItem(root, "email")) {
        sqlite3 *db = *(mx_get_db());
        gchar *first_name = cJSON_GetObjectItem(root, "first_name")->valuestring;
        gchar *last_name = cJSON_GetObjectItem(root, "last_name")->valuestring;
        gchar *email = cJSON_GetObjectItem(root, "email")->valuestring;
        int status = 0;

        profile_setup_query(db, first_name, last_name, email);

        send_response(1, client);
    }
    g_warning("Not valid request\n");
}
