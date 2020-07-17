#include "server.h"

static gchar *push_login(sqlite3 *db, cJSON *root, gchar *password) {
    gchar *phone = cJSON_GetObjectItem(root, "phone")->valuestring;
    gchar *query = "INSERT INTO users_credential(phone, auth_token) \
                   VALUES(?, ?);";
    sqlite3_stmt *stmt = NULL;
    gchar *token = mx_create_token(phone, password);
    gint rc = 0;

    if ((rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0)) != SQLITE_OK)
        g_warning("sign_up_query prepare: %s\n", sqlite3_errmsg(db));
    if ((sqlite3_bind_text(stmt, 1, phone, -1, NULL)) != SQLITE_OK)
        g_warning("phone bind: %s\n", sqlite3_errmsg(db));
    if ((sqlite3_bind_text(stmt, 2, token, -1, NULL)) != SQLITE_OK)
        g_warning("auth_token bind: %s\n", sqlite3_errmsg(db));
    if ((rc = sqlite3_step(stmt)) != SQLITE_DONE)
        g_warning("push_login step: %s\n", sqlite3_errmsg(db));
    if ((rc = sqlite3_finalize(stmt)) != SQLITE_OK)
        g_warning("push_login finalize: %s\n", sqlite3_errmsg(db));
    return token;
}

static gint mx_get_user_id(cJSON *root, sqlite3 *db) {
    gchar *quary = "SELECT user_id FROM users_credential WHERE phone = ?;";
    gchar *phone = cJSON_GetObjectItem(root, "phone")->valuestring;
    sqlite3_stmt *stmt = NULL;
    gint rc = 0;
    gint user_id = 0;

    if ((rc = sqlite3_prepare_v2(db, quary, -1, &stmt, NULL)) != SQLITE_OK)
        g_warning("check_if_excist prepare: %s\n", sqlite3_errstr(rc));
    if ((sqlite3_bind_text(stmt, 1, phone, -1, NULL)) != SQLITE_OK)
        g_warning("check_if_excist bind: phone:%s %s\n",
                  phone, sqlite3_errstr(rc));
    if ((rc = sqlite3_step(stmt)) == SQLITE_DONE)
        user_id = -1;
    else if (rc == SQLITE_ROW)
        user_id = sqlite3_column_int(stmt, 0);
    else
        g_warning("get_user_id_run step rc:%d\n", rc);
    if ((rc = sqlite3_finalize(stmt)) != SQLITE_OK)
        g_warning("get_user_id_run finalize rc:%d\n", rc);
    return user_id;
}

static gint push_profile_info(sqlite3 *db, gchar *user_id, gchar *name,
                              gchar *last_name, gchar *email) {
    gchar *query = "INSERT INTO user_profile(user_id, name, last_name, email) \
                   VALUES(?, ?, ?, ?);";
    sqlite3_stmt *stmt = NULL;
    gint rc = 0;

    if ((rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0)) != SQLITE_OK)
        g_warning("profile_setup prepare: %s\n", sqlite3_errmsg(db));
    if ((sqlite3_bind_text(stmt, 1, user_id, -1, NULL)) != SQLITE_OK)
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

gchar *mx_add_user_to_bd(cJSON *root, t_client *client, sqlite3 *db) {
    gchar *name = cJSON_GetObjectItem(root, "first_name")->valuestring;
    gchar *last_name = cJSON_GetObjectItem(root, "last_name")->valuestring;
    gchar *email = cJSON_GetObjectItem(root, "email")->valuestring;
    gchar *token = push_login(db, root, client->password);
    gchar *user_id = NULL;

    g_sprintf(user_id, "%d", mx_get_user_id(root, db));
    push_profile_info(db, user_id, name, last_name, email);
    return token;
}
