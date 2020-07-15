#include "server.h"

static void create_users_credential(sqlite3 **db) {
    gchar *errmsg = NULL;
    int rc = 0;

    if ((rc = sqlite3_exec(*db, "SELECT * FROM users_credential LIMIT 1;",
                    NULL, NULL, &errmsg)) != SQLITE_OK) {
        if ((rc = sqlite3_exec(*db, "CREATE TABLE users_credential (\
                            user_id INTEGER,\
                            login TEXT NOT NULL,\
                            passwd TEXT NOT NULL,\
                            temp_passwd TEXT,\
                            auth_token TEXT,\
                            PRIMARY KEY(user_id));", NULL, NULL, &errmsg))
                            != SQLITE_OK) {
                sqlite3_close(*db);
                g_error("Failed to create \"users_credential\": %s\n", errmsg);
        }
    }
}

static void create_user_profile(sqlite3 **db) {
    gchar *errmsg = NULL;
    int rc = 0;

    if ((rc = sqlite3_exec(*db, "SELECT * FROM user_profile LIMIT 1;",
                    NULL, NULL, &errmsg)) != SQLITE_OK) {
        if ((rc = sqlite3_exec(*db, "CREATE TABLE user_profile (\
                            user_id INTEGER,\
                            name TEXT,\
                            photo BLOB,\
                            FOREIGN KEY(user_id)\
                            REFERENCES users_credential(user_id));",
                            NULL, NULL, &errmsg))
                            != SQLITE_OK) {
                sqlite3_close(*db);
                g_error("Failed to create \"user_profile\": %s\n", errmsg);
        }
    }
}

static void create_chats(sqlite3 **db) {
    gchar *errmsg = NULL;
    int rc = 0;

    if ((rc = sqlite3_exec(*db, "SELECT * FROM chats LIMIT 1;",
                    NULL, NULL, &errmsg)) != SQLITE_OK) {
        if ((rc = sqlite3_exec(*db, "CREATE TABLE chats (\
                            chat_id INTEGER,\
                            name TEXT,\
                            PRIMARY KEY(chat_id));", NULL, NULL, &errmsg))
                            != SQLITE_OK) {
                sqlite3_close(*db);
                g_error("Failed to create \"chats\": %s\n", errmsg);
        }
    }
}

static void create_messages(sqlite3 **db) {
    gchar *errmsg = NULL;
    int rc = 0;

    if ((rc = sqlite3_exec(*db, "SELECT * FROM messages LIMIT 1;",
                    NULL, NULL, &errmsg)) != SQLITE_OK) {
        if ((rc = sqlite3_exec(*db, "CREATE TABLE messages (\
                            msg_id INTEGER,\
                            user_id INTEGER,\
                            chat_id INTEGER,\
                            message TEXT,\
                            data INTEGER,\
                            FOREIGN KEY(user_id)\
                            REFERENCES users_credential(user_id),\
                            FOREIGN KEY(chat_id)\
                            REFERENCES chats(chat_id),\
                            PRIMARY KEY(msg_id));", NULL, NULL, &errmsg))
                            != SQLITE_OK) {
                sqlite3_close(*db);
                g_error("Failed to create \"messages\": %s\n", errmsg);
        }
    }
}

static void create_user_in_chats(sqlite3 **db) {
    gchar *errmsg = NULL;
    int rc = 0;

    if ((rc = sqlite3_exec(*db, "SELECT * FROM user_in_chat LIMIT 1;",
                    NULL, NULL, &errmsg)) != SQLITE_OK) {
        if ((rc = sqlite3_exec(*db, "CREATE TABLE user_in_chat (\
                            chat_id INTEGER,\
                            user_id INTEGER,\
                            FOREIGN KEY(chat_id)\
                            REFERENCES chats(chat_id));", NULL, NULL, &errmsg))
                            != SQLITE_OK) {
                sqlite3_close(*db);
                g_error("Failed to create \"user_in_chat\": %s\n", errmsg);
        }
    }
}

static void create_user_contact_list(sqlite3 **db) {
    gchar *errmsg = NULL;
    int rc = 0;

    if ((rc = sqlite3_exec(*db, "SELECT * FROM user_contact_list LIMIT 1;",
                    NULL, NULL, &errmsg)) != SQLITE_OK) {
        if ((rc = sqlite3_exec(*db, "CREATE TABLE user_contact_list (\
                            user_id INTEGER,\
                            contact INTEGER,\
                            FOREIGN KEY(user_id)\
                            REFERENCES chats(user_id));", NULL, NULL, &errmsg))
                            != SQLITE_OK) {
                sqlite3_close(*db);
                g_error("Failed to create \"user_contact_list\": %s\n", errmsg);
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
    if ((rc = sqlite3_exec(*db, "PRAGMA foreign_keys = ON;",
                    NULL, NULL, &errmsg)) != SQLITE_OK) {
        g_error("Failed to enable foreign_keys, %s", errmsg);
        sqlite3_close(*db);
    }
    create_users_credential(db);
    create_chats(db);
    create_messages(db);
    create_user_contact_list(db);
    create_user_profile(db);
    create_user_in_chats(db);
}