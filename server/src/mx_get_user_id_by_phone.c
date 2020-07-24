#include "server.h"

gint mx_get_user_id_by_phone(gchar *phone, sqlite3 *db) {
    gchar *query = "SELECT user_id FROM users_credential WHERE phone = ?;";
    sqlite3_stmt *stmt = NULL;
    gint rc = 0;
    gint user_id = 0;

    g_print("phone = %s\n", phone);
    if ((rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL)) != SQLITE_OK)
        g_warning("mx_get_user_id_by_phone prepare: %d, %s\n", rc,
                  sqlite3_errmsg(db));
    if ((sqlite3_bind_text(stmt, 1, phone, -1, NULL)) != SQLITE_OK)
        g_warning("mx_get_user_id_by_phone  bind: phone:%s %d\n",
                  phone, rc);
    if ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
        user_id = sqlite3_column_int(stmt, 0);
    else if (rc == SQLITE_DONE)
        user_id = 0;
    else
        g_warning("mx_get_user_id_by_phone step rc:%d\n", rc);

    if ((rc = sqlite3_finalize(stmt)) != SQLITE_OK)
        g_warning("mx_get_user_id_by_phone finalize rc:%d\n", rc);
    return user_id;
}
