#include "server.h"

gint mx_put_message_in_db_run(sqlite3_stmt *stmt) {
    gint rc = 0;
    sqlite3 *db = *(mx_get_db());

    if ((rc = sqlite3_step(stmt)) == SQLITE_OK)
        g_message("mx_put_message_in_db_run SUCCESS\n");
    else
        g_warning("mx_put_message_in_db_run step rc:%d, %s\n", rc,
                  sqlite3_errmsg(db));
    if ((rc = sqlite3_finalize(stmt)) != SQLITE_OK)
        g_warning("mx_put_message_in_db_run finalize rc:%d\n", rc);
    return rc;
}

gint64 mx_get_chat_id(gint32 uid1, gint32 uid2) {
    gint64 chat_id = 0;
    if (uid1 > uid2) {
        chat_id = uid2;
        uid2 = uid1;
    }
    else
        chat_id = uid1;
    chat_id = chat_id << 32;
    chat_id += (gint64)uid2;
    return chat_id;
}

gint mx_put_message_in_db_prepare(cJSON *root, sqlite3_stmt **stmt, gint sender_id) { //auditor
    if (sender_id == 0) {
        g_message("Please log_in\n");
        return -1;
    }
    sqlite3 *db = *(mx_get_db());
    gchar *query = "INSERT INTO messages(message, sender_id, receiver_id,\
                    chat_id, date) VALUES(?, ?, ?, ?, ?);";
    gchar *message = cJSON_GetObjectItem(root, "message")->valuestring;
    gint receiver_id = cJSON_GetObjectItem(root, "receiver_id")->valueint;
    gint64 chat_id = mx_get_chat_id(sender_id, receiver_id);
    gint date = g_get_real_time() / 1000000;
    g_print("%s %d %d\n", message, receiver_id, date);
    gint rc = 0;

    if ((rc = sqlite3_prepare_v2(db, query, -1, stmt, NULL)) != SQLITE_OK)
        g_warning("mx_put_message_in_db_prepare prepare: %d, %s\n", rc,
                  sqlite3_errmsg(db));
    if ((rc = sqlite3_bind_text(*stmt, 1, message, -1, NULL)) != SQLITE_OK)
        g_warning("mx_put_message_in_db_prepare bind: message:%s %d\n",
                  message, rc);
    if ((rc = sqlite3_bind_int(*stmt, 2, sender_id)) != SQLITE_OK)
        g_warning("mx_put_message_in_db_prepare bind: sender_id:%d %d\n",
                  sender_id, rc);
    if ((rc = sqlite3_bind_int(*stmt, 3, receiver_id)) != SQLITE_OK)
        g_warning("mx_put_message_in_db_prepare bind: receiver_id:%d %d\n",
                  receiver_id, rc);
    if ((rc = sqlite3_bind_int64(*stmt, 4, chat_id)) != SQLITE_OK)
        g_warning("mx_put_message_in_db_prepare bind: chat_id:%lld %d\n",
                  chat_id, rc);
    if ((rc = sqlite3_bind_int(*stmt, 5, date)) != SQLITE_OK)
        g_warning("mx_put_message_in_db_prepare bind: date:%d %d\n",
                  date, rc);
    return rc;
}

static gboolean is_valid(cJSON *root) {
    if (!cJSON_GetObjectItem(root, "receiver_id"))
        return FALSE;
    if (!cJSON_GetObjectItem(root, "message"))
        return FALSE;
    return TRUE;
}

void mx_send_message(cJSON *root, t_client *client) {
    sqlite3_stmt *stmt = NULL;
    gint rc = 0;

    if (is_valid(root) == FALSE) {
        g_warning("Invalid send_message request\n");
        return;
    }
    if ((rc = mx_put_message_in_db_prepare(root, &stmt, client->uid)) != SQLITE_OK)
        g_warning("mx_put_message_in_db_prepare failed: %d\n", rc);
        // TODO: send error?;
    if ((rc = mx_put_message_in_db_run(stmt)) != SQLITE_OK)
        g_warning("mx_put_message_in_db_run failed: %d\n", rc);
        // TODO: send error?;
    else
        mx_send_data(client->data_out, "message sent\n");
    return;
}
