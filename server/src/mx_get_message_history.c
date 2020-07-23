#include "server.h"

gint mx_get_chat_history_prepare(cJSON *root,
                                    sqlite3_stmt **stmt,
                                    gint sender_id) {
    if (sender_id == 0) {
        g_message("Please login\n");
        return -1;
    }
    sqlite3 *db = *(mx_get_db());
    gchar *query = "SELECT message, sender_id, receiver_id, date FROM messages\
                    WHERE chat_id = ?;";
    gint receiver_id = cJSON_GetObjectItem(root, "receiver_id")->valueint;
    gint64 chat_id = mx_get_chat_id(sender_id, receiver_id);
    gint rc = 0;

    if ((rc = sqlite3_prepare_v2(db, query, -1, stmt, NULL)) != SQLITE_OK)
        g_warning("mx_get_chat_history_prepare prepare: %d, %s\n", rc,
                  sqlite3_errmsg(db));
    if ((rc = sqlite3_bind_int64(*stmt, 1, chat_id)) != SQLITE_OK)
        g_warning("mx_get_chat_history_prepare  bind: chat_id:%d %d\n",
                  receiver_id, rc);
    return rc;
}

gchar *mx_form_chat_history_response(sqlite3_stmt *stmt) {
    cJSON *chat_history = cJSON_CreateObject();
    cJSON *message_arr = cJSON_CreateArray();
    gchar *response = NULL;
    gint rc = 0;

    while (rc != SQLITE_DONE) {
        cJSON *message = cJSON_CreateObject();
        cJSON_AddStringToObject(message, "message",
                                (gchar*)sqlite3_column_text(stmt, 0));
        cJSON_AddNumberToObject(message, "sender_id",
                                sqlite3_column_int(stmt, 1));
        cJSON_AddNumberToObject(message, "receiver_id",
                                sqlite3_column_int(stmt, 2));
        cJSON_AddNumberToObject(message, "date",
                                sqlite3_column_int(stmt, 3));
        cJSON_AddItemToArray(message_arr, message);
        rc = sqlite3_step(stmt);
    }
    response = cJSON_PrintUnformatted(message_arr);
    return response;
}

gchar *mx_get_chat_history_run(sqlite3_stmt *stmt) {
    gint rc = 0;
    sqlite3 *db = *(mx_get_db());
    gchar *response = NULL;

    if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        response = mx_form_chat_history_response(stmt);
    }
    else if (rc == SQLITE_DONE)
        g_warning("mx_get_chat_history_run step: no message for chat_id\n");
    else
        g_warning("mx_put_chat_in_db_run step rc:%d, %s\n",
                  rc, sqlite3_errmsg(db));
    if ((rc = sqlite3_finalize(stmt)) != SQLITE_OK)
        g_warning("mx_get_chat_history_run finalize rc:%d\n", rc);
    return response;
}

static gboolean is_valid(cJSON *root) {
    cJSON *receiver_id = cJSON_GetObjectItemCaseSensitive(root, "receiver_id");

    if (!cJSON_IsNumber(receiver_id))
        return FALSE;
    return TRUE;
}

void mx_get_chat_history(cJSON *root, t_client *client) {
    sqlite3_stmt *stmt = NULL;
    gint rc = 0;
    gchar *response = NULL;

    if (is_valid(root) == FALSE) {
        g_warning("Invalid get_chat_history request\n");
        return;
    }
    if ((rc = mx_get_chat_history_prepare(root, &stmt, client->uid))
        != SQLITE_OK)
        g_warning("mx_get_chat_history_prepare failed: %d\n", rc);
        // TODO: send error?;
    if ((response = mx_get_chat_history_run(stmt)) == NULL)
        g_warning("mx_put_chat_in_db_run failed");
        // TODO: send error?;
    else
        mx_send_data(client->data_out, response);
    return;
}
