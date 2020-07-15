#include "server.h"

static void create_users_credential(db) {
    int rc = 0;

    if ((rc = sqlite3_exec(*db, "SELECT * FROM users_credential LIMIT 1;",
                    NULL, NULL, &errmsg)) != SQLITE_OK) {
        if ((rc = sqlite3_exec(*db, "CREATE TABLE users_credential (\
                            user_id INTEGER,\
                            login VARCHAR(50) NOT NULL,\
                            passwd VARCHAR(32) NOT NULL,\
                            temp_passwd VARCHAR(32) NOT NULL,\
                            auth_token VARCHAR(64) NOT NULL,\
                            PRIMARY KEY(user_id));", NULL, NULL, &errmsg))
                            != SQLITE_OK) {
                sqlite3_close(*db);
                g_error("Failed to create \"users_credential\": %s\n", errmsg);
        }
    }
}

static void create_user_profile(db) {
    int rc = 0;

    if ((rc = sqlite3_exec(*db, "SELECT * FROM user_profile LIMIT 1;",
                    NULL, NULL, &errmsg)) != SQLITE_OK) {
        if ((rc = sqlite3_exec(*db, "CREATE TABLE user_profile (\
                            user_id INTEGER,\
                            name VARCHAR(50),\
                            photo BLOB,\
                            FOREIGN KEY(user_id))\
                            REFERENCES users_credential(user_id);",
                            NULL, NULL, &errmsg))
                            != SQLITE_OK) {
                sqlite3_close(*db);
                g_error("Failed to create \"user_profile\": %s\n", errmsg);
        }
    }
}

static void create_chats(db) {
    int rc = 0;

    if ((rc = sqlite3_exec(*db, "SELECT * FROM chats LIMIT 1;",
                    NULL, NULL, &errmsg)) != SQLITE_OK) {
        if ((rc = sqlite3_exec(*db, "CREATE TABLE chats (\
                            chat_id INTEGER,\
                            name VARCHAR(50),\
                            PRIMARY KEY(user_id));", NULL, NULL, &errmsg))
                            != SQLITE_OK) {
                sqlite3_close(*db);
                g_error("Failed to create \"chats\": %s\n", errmsg);
        }
    }
}

static void create_user_in_chat(db) {
    int rc = 0;

    if ((rc = sqlite3_exec(*db, "SELECT * FROM messages LIMIT 1;",
                    NULL, NULL, &errmsg)) != SQLITE_OK) {
        if ((rc = sqlite3_exec(*db, "CREATE TABLE messages (\
                             INTEGER,\
                            user_id INTEGER,\
                            contact INTEGER,\
                            data INTEGER,\
                            PRIMARY KEY(user_id));", NULL, NULL, &errmsg))
                            != SQLITE_OK) {
                sqlite3_close(*db);
                g_error("Failed to create \"messages\": %s\n", errmsg);
        }
    }
}

void mx_db_init(void) {
    gchar *errmsg = NULL;
    sqlite3 **db = mx_get_db();
    int rc = 0;

    if ((rc = sqlite3_open("uchat_db", db)) != SQLITE_OK) {
            g_error("Can't open database: %s\n", sqlite3_errmsg(*db));
            sqlite3_close(*db);
    }
}
