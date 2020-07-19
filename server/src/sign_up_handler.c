#include "server.h"

gint mx_sign_up_query(cJSON *root, sqlite3 *db) {
    gchar *query = "INSERT INTO users_credential(login, passwd) \
                    VALUES(?, ?);";
    sqlite3_stmt *stmt = NULL;
    gchar *login = cJSON_GetObjectItem(root, "login")->valuestring;
    gchar *passwd = cJSON_GetObjectItem(root, "password")->valuestring;
    gint rc = 0;

    if ((rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0)) != SQLITE_OK)
        g_warning("sign_up_query prepare: %s\n", sqlite3_errmsg(db));
    if ((sqlite3_bind_text(stmt, 1, login, -1, NULL)) != SQLITE_OK)
        g_warning("sign_up_query bind:\nlogin:%s\n%s\n",
                   login, sqlite3_errmsg(db));
    if ((sqlite3_bind_text(stmt, 2, passwd, -1, NULL)) != SQLITE_OK)
        g_warning("sign_up_query bind: login:%s\n%s\n",
                   login, sqlite3_errmsg(db));
    if ((rc = sqlite3_step(stmt)) != SQLITE_DONE)
        g_warning("sign_up_query step: %s\n", sqlite3_errmsg(db));
    if ((rc = sqlite3_finalize(stmt)) != SQLITE_OK)
        g_warning("sign_up_query finalize: %s\n", sqlite3_errmsg(db));
    return rc;
}

gboolean mx_check_if_user_excist(cJSON *root, sqlite3 *db) {
    gchar *query = "SELECT login FROM users_credential WHERE login = ?;";
    sqlite3_stmt *stmt = NULL;
    gchar *login = cJSON_GetObjectItem(root, "login")->valuestring;
    gboolean result = 0;
    gint rc = 0;

    if ((rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL)) != SQLITE_OK)
        g_warning("check_if_excist prepare: %s\n", sqlite3_errstr(rc));
    if ((sqlite3_bind_text(stmt, 1, login, -1, NULL)) != SQLITE_OK)
        g_warning("check_if_excist bind: login:%s %s\n",
                   login, sqlite3_errstr(rc));
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

static gboolean is_valid(cJSON *root) {
    if (cJSON_GetObjectItem(root, "login") == NULL)
        return FALSE;
    if (cJSON_GetObjectItem(root, "password") == NULL)
        return FALSE;
    return TRUE;
}

void mx_sign_up(cJSON *root, t_client *client) {
    sqlite3 *db = *(mx_get_db());

    if (is_valid(root) == FALSE) {
        g_warning("Invalid sign_up request\n");
        return;
    }
    if (mx_check_if_user_excist(root, db) == false) {
        mx_sign_up_query(root, db);
        g_print("User not exsist\n");
    }
    else {
        g_print("User exsist\n");
    }

    (void)client; // Need to use to send response
}

