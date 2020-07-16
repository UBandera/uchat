#include "server.h"

gint mx_recovery_query(cJSON *root, sqlite3 *db) {
    gchar *query = "UPDATE users_credential SET recovery_passwd = ? WHERE login = ?;";
    sqlite3_stmt *stmt = NULL;
    gchar *login = cJSON_GetObjectItem(root, "login")->valuestring;
    gint rc = 0;

    if ((rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0)) != SQLITE_OK)
        g_warning("recovery_password_query prepare: %s\n", sqlite3_errmsg(db));
    if ((sqlite3_bind_text(stmt, 1, "12345678", -1, NULL)) != SQLITE_OK)
        g_warning("recovery_password_query bind:\nrec_pass:%s\n%s\n",
                   login, sqlite3_errmsg(db));
    if ((sqlite3_bind_text(stmt, 2, login, -1, NULL)) != SQLITE_OK)
        g_warning("recovery_password_query bind:\nlogin:%s\n%s\n",
                   login, sqlite3_errmsg(db));
    if ((rc = sqlite3_step(stmt)) != SQLITE_DONE)
        g_warning("recovery_password_query step: %s\n", sqlite3_errmsg(db));
    if ((rc = sqlite3_finalize(stmt)) != SQLITE_OK)
        g_warning("recovery_password_query finalize: %s\n", sqlite3_errmsg(db));
    return rc;
}


void mx_recovery_password(cJSON *root, t_client *client) {
    sqlite3 *db = *(mx_get_db());

    if (!mx_check_if_user_excist(root, db)) {
        g_print("User not exsist\n");
    }
    else {
        // char *username = "SELECT username FROM users_credential WHERE login = ?;"
        gchar *login = cJSON_GetObjectItem(root, "login")->valuestring;
        char *body = mx_recovery_body(login); // must be username from bd against login

        g_print("User exsist\n");
        mx_recovery_query(root, db);
        mx_send_mail(login, body);
        mx_send_data(client->data_out, "mail was send\n");
    }
}
