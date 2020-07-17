#include "server.h"

static gint mx_sign_up_query(sqlite3 *db, gchar *phone) {
    gchar *query = "INSERT INTO users_credential(phone) VALUES(?);";
    sqlite3_stmt *stmt = NULL;
    gint rc = 0;

    if ((rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0)) != SQLITE_OK)
        g_warning("sign_up_query prepare: %s\n", sqlite3_errmsg(db));
    if ((sqlite3_bind_text(stmt, 1, phone, -1, NULL)) != SQLITE_OK)
        g_warning("sign_up_query bind:\nphone:%s\n%s\n",
                   phone, sqlite3_errmsg(db));
    if ((rc = sqlite3_step(stmt)) != SQLITE_DONE)
        g_warning("sign_up_query step: %s\n", sqlite3_errmsg(db));
    if ((rc = sqlite3_finalize(stmt)) != SQLITE_OK)
        g_warning("sign_up_query finalize: %s\n", sqlite3_errmsg(db));
    return rc;
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

static void send_response(int status, t_client *client, gchar *token) {
    gchar *response = NULL;

    if (!status)
        response = mx_send_error_response(ER_PASS, "Invalid password");
    else if (status == 1) {
        response = mx_auth_send_response(token);
    }
    else
        response = user_not_exist_response();
    mx_send_data(client->data_out, response);
    g_free(response);
}

static gchar *setup_token(sqlite3 *db, gchar *phone, gchar *password) {
    gchar *query = "UPDATE users_credential SET auth_token = ? \
                   WHERE phone = ?;";
    sqlite3_stmt *stmt = NULL;
    gchar *token = mx_create_token(phone, password);
    gint rc = 0;

    if ((rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0)) != SQLITE_OK)
        g_warning("sign_up_query prepare: %s\n", sqlite3_errmsg(db));
    if ((sqlite3_bind_text(stmt, 1, token, -1, NULL)) != SQLITE_OK)
        g_warning("phone bind: %s\n", sqlite3_errmsg(db));
    if ((sqlite3_bind_text(stmt, 2, phone, -1, NULL)) != SQLITE_OK)
        g_warning("auth_token bind: %s\n", sqlite3_errmsg(db));
    if ((rc = sqlite3_step(stmt)) != SQLITE_DONE)
        g_warning("push_login step: %s\n", sqlite3_errmsg(db));
    if ((rc = sqlite3_finalize(stmt)) != SQLITE_OK)
        g_warning("push_login finalize: %s\n", sqlite3_errmsg(db));
    return token;
}

void mx_auth_request_handler(cJSON *root, t_client *client) {
    if (cJSON_GetObjectItem(root, "password")) {
        sqlite3 *db = *(mx_get_db());
        gchar *phone = cJSON_GetObjectItem(root, "phone")->valuestring;
        gchar *password = cJSON_GetObjectItem(root, "password")->valuestring;
        int status = 0;
        gchar *token = NULL;

        if (!g_strcmp0(password, client->password)) {
            if (mx_check_if_user_excist(phone, db)) {
                setup_token(db, phone, password);
                status = 1;
            }
            else {
                mx_sign_up_query(db, phone);
                status = 2;
            }
        }
        send_response(status, client, token);
        return;
    }
    g_warning("Not valid request\n");
}
