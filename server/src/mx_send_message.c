#include "server.h"

gint mx_put_message_in_db_run(sqlite3_stmt *stmt, t_client *client) {
    gint rc = 0;

    if ((rc = sqlite3_step(stmt)) == SQLITE_OK)
        g_message("mx_put_message_in_db_run SUCCESS\n");
    else
        g_warning("mx_put_message_in_db_run step rc:%d\n", rc);
    if ((rc = sqlite3_finalize(stmt)) != SQLITE_OK)
        g_warning("mx_put_message_in_db_run finalize rc:%d\n", rc);
    return rc;
}

gint mx_put_message_in_db_prepare(cJSON *root, sqlite3_stmt **stmt) { //auditor
    sqlite3 *db = *(mx_get_db());
    gchar *query = "INSERT INTO messages(message, user_id, chat_id,\
                    send_time) VALUES(?, ?, ?, ?);";
    gchar *message = cJSON_GetObjectItem(root, "message")->string;
    gchar *user_id = cJSON_GetObjectItem(root, "user_id")->string;
    gchar *chat_id = cJSON_GetObjectItem(root, "chat_id")->string;
    gchar *send_time = cJSON_GetObjectItem(root, "send_time")->string;
    g_print("%s %s %s %s\n", message, user_id, chat_id, send_time);
    gint rc = 0;

    if ((rc = sqlite3_prepare_v2(db, query, -1, stmt, NULL)) != SQLITE_OK)
        g_warning("mx_put_message_in_db_prepare prepare: %d, %s\n", rc,
                  sqlite3_errmsg(db));
    if ((rc = sqlite3_bind_text(*stmt, 1, message, -1, NULL)) != SQLITE_OK)
        g_warning("mx_put_message_in_db_prepare bind: message:%s %d\n",
                  message, rc);
    if ((rc = sqlite3_bind_text(*stmt, 2, user_id, -1, NULL)) != SQLITE_OK)
        g_warning("mx_put_message_in_db_prepare bind: user_id:%s %d\n",
                  user_id, rc);
    if ((rc = sqlite3_bind_text(*stmt, 3, chat_id, -1, NULL)) != SQLITE_OK)
        g_warning("mx_put_message_in_db_prepare bind: chat_id:%s %d\n",
                  chat_id, rc);
    if ((rc = sqlite3_bind_text(*stmt, 4, send_time, -1, NULL)) != SQLITE_OK)
        g_warning("mx_put_message_in_db_prepare bind: send_time:%s %d\n",
                  send_time, rc);
    return rc;
}

static gboolean is_valid(cJSON *root) {
    if (!cJSON_GetObjectItem(root, "user_id"))
        return FALSE;
    if (!cJSON_GetObjectItem(root, "chat_id"))
        return FALSE;
    if (!cJSON_GetObjectItem(root, "send_time"))
        return FALSE;
    if (!cJSON_GetObjectItem(root, "message"))
        return FALSE;
    return TRUE;
}

void mx_send_message(cJSON *root, t_client *client) {
    sqlite3_stmt *stmt = NULL;

    if (is_valid(root) == FALSE) {
        g_warning("Invalid send_message request\n");
        return;
    }
    if (mx_put_message_in_db_prepare(root, &stmt) != SQLITE_OK)
        g_warning("mx_put_message_in_db_prepare failed\n");
        // TODO: send error?;
    if (mx_put_message_in_db_run(stmt, client) != SQLITE_OK)
        g_warning("mx_put_message_in_db_run failed\n");
        // TODO: send error?;
    else
        mx_send_data(client->data_out, "message sent\n");
    return;
}

