#include "server.h"
#include "glib/gprintf.h"

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

    g_sprintf(user_id, "%d", mx_get_user_id_by_phone(
              cJSON_GetObjectItem(root, "phone")->valuestring, db));
    push_profile_info(db, user_id, name, last_name, email);
    return token;
}
