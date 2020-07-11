#include "server.h"

void mx_db_init(void) {
    gchar *errmsg = NULL;
    sqlite3 **db = mx_get_db();
    int rc = 0;

    if ((rc = sqlite3_open("uchat_db", db)) != SQLITE_OK) {
            g_error("Can't open database: %s\n", sqlite3_errmsg(*db));
            sqlite3_close(*db);
    }
    if ((rc = sqlite3_exec(*db, "SELECT User_id FROM users_credential LIMIT 1;",
                    NULL, NULL, &errmsg)) != SQLITE_OK) {
        if ((rc = sqlite3_exec(*db, "CREATE TABLE users_credential ( \
                            user_id INTEGER, \
                            login VARCHAR(50) NOT NULL, \
                            passwd_hash VARCHAR(32) NOT NULL, \
                            recovery_passwd VARCHAR(32), \
                            PRIMARY KEY(user_id));", NULL, NULL, &errmsg)) != SQLITE_OK) {
                sqlite3_close(*db);
                g_error("Failed to create table: %s\n", errmsg);
        }
    }
}
