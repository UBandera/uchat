#include "server.h"

gboolean mx_check_if_user_excist(const gchar *phone, sqlite3 *db) {
    gchar *query = "SELECT phone FROM users_credential WHERE phone = ?;";
    sqlite3_stmt *stmt = NULL;
    gboolean result = 0;
    gint rc = 0;

    if ((rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL)) != SQLITE_OK)
        g_warning("check_if_excist prepare: %s\n", sqlite3_errstr(rc));
    if ((sqlite3_bind_text(stmt, 1, phone, -1, NULL)) != SQLITE_OK)
        g_warning("check_if_excist bind: login:%s %s\n",
                  phone, sqlite3_errstr(rc));
    if ((rc = sqlite3_step(stmt)) == SQLITE_DONE)
        result = false;
    else if (rc == SQLITE_ROW)
        result = true;
    else
        g_warning("check_if_excist step rc:%d\n", rc);
    if ((rc = sqlite3_finalize(stmt)) != SQLITE_OK)
        g_warning("check_if_excist finalize rc:%d\n", rc);
    return result;
}

static gchar *password_send_response(void) {
    cJSON *json = cJSON_CreateObject();
    gchar *message = "Password has successfully sent.";
    gchar *response = NULL;

    cJSON_AddItemToObject(json,
                          "response_type",
                          cJSON_CreateNumber(RS_PASSWORD_SENT));
    cJSON_AddItemToObject(json, "message", cJSON_CreateString(message));
    response = cJSON_PrintUnformatted(json);
    if (!response){
        g_warning("Failed to print make request.\n");
    }
    cJSON_Delete(json);
    return response;
}

static void send_response(gint status, t_client *client) {
    gchar *response = NULL;

    if (!status)
        response = password_send_response();
    else
        response = mx_send_error_response(ER_SENT_PASS,
                                          "Sending password error");
    mx_send_data(client->data_out, response);
    g_free(response);
}

void mx_password_request_handler(cJSON *root, t_client *client) {
    if (cJSON_GetObjectItem(root, "phone")) {
        // sqlite3 *db = *(mx_get_db());
        gchar *phone = cJSON_GetObjectItem(root, "phone")->valuestring;
        gint status = 0;
        gchar *body = NULL;
        gchar *password = mx_generate_password();

        client->password = g_compute_checksum_for_string(
                         G_CHECKSUM_SHA256, password, strlen(password));
        // if (!mx_check_if_user_excist(phone, db)) {
            body = mx_recovery_body("ARTEM", password);
            g_print("body = %s\n", body);
            status = mx_send_mail("shemedvedd@gmail.com", body);
        // }
        // else {
            // body = mx_create_sms_body(phone, password);
            // status = mx_send_sms(body);
        // }
        send_response(status, client);
        g_free(body);
        g_free(password);
        return;
    }
    g_warning("Not valid request\n");
}
