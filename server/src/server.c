#include "mx_server.h"

const gboolean NEW_USER = 0;

gint64 gui = 1;
gint request_count = 0;

void print_hash_table(gpointer key, gpointer value, gpointer user_data) {
    g_print("Connected user id is %lld\n", *(gint64 *)key);
}

sqlite3 **mx_get_db(void) {
    static sqlite3 *db = NULL;

    return &db;
}

int mx_sign_up_quary(cJSON *root, sqlite3 *db) {
    gchar *quary = "INSERT INTO users_credential(login, passwd_hash), \
                    VALUE(?, ?);";
    sqlite3_stmt *stmt = NULL;
    gchar *login = cJSON_GetObjectItem(root, "login")->valuestring;
    gchar *passwd = cJSON_GetObjectItem(root, "passwd_hash")->valuestring;
    int rc = 0; // should move to the function arguments );

    if ((rc = sqlite3_prepare_v2(db, quary, -1, &stmt, 0)) != SQLITE_OK)
        g_warning("sign_up_quary prepare: %s\n", sqlite3_errmsg(db));
    if ((sqlite3_bind_text(stmt, 1, login, -1, NULL)) != SQLITE_OK)
        g_warning("sign_up_quary bind:\nlogin:%s\n%s\n",
                   login, sqlite3_errmsg(db));
    if ((sqlite3_bind_text(stmt, 2, passwd, -1, NULL)) != SQLITE_OK)
        g_warning("sign_up_quary bind: login:%s\n%s\n",
                   login, sqlite3_errmsg(db));
    if ((rc = sqlite3_step(stmt)) != SQLITE_OK)
        g_warning("sign_up_quary step: %s\n", sqlite3_errmsg(db));
    if ((rc = sqlite3_finalize(stmt)) != SQLITE_OK)
        g_warning("sign_up_quary finalize: %s\n", sqlite3_errmsg(db));
    return rc;
}

gboolean mx_check_if_user_excist(cJSON *root, sqlite3 *db) {
    gchar *quary = "SELECT login FROM users_credential WHERE login = ?;";
    sqlite3_stmt *stmt = NULL;
    gchar *login = cJSON_GetObjectItem(root, "login")->valuestring;
    const guchar *result = NULL;
    gchar *errmsg = NULL;
    int rc = 0;

    if ((rc = sqlite3_prepare_v2(db, quary, -1, &stmt, NULL)) != SQLITE_OK)
        g_warning("check_if_excist prepare: %s\n", sqlite3_errstr(rc));
    if ((sqlite3_bind_text(stmt, 1, login, -1, NULL)) != SQLITE_OK)
        g_warning("check_if_excist bind: login:%s %s\n",
                   login, sqlite3_errstr(rc));
    if ((rc = sqlite3_step(stmt)) != SQLITE_OK)
        g_warning("check_if_excist step rc: %d, %s\n", rc, sqlite3_errstr(rc));
    if ((result = sqlite3_column_text(stmt, 1)) == NULL)
        g_warning("check_if_excist fetch rc: %d %s\n", rc, sqlite3_errstr(rc));
    if ((rc = sqlite3_finalize(stmt)) != SQLITE_OK)
        g_warning("check_if_excist finalize rc:%d %s\n", rc, sqlite3_errstr(rc));
    return true;
}

void mx_sign_up(cJSON *root, t_client *client) {
    sqlite3 *db = *(mx_get_db());

    if (mx_check_if_user_excist(root, db) == false) {
        mx_sign_up_quary(root, db);
        g_print("User not exsist\n");
    }
    else {
        g_print("User exsist\n");
    }

    (void)client;
}

// void sign_up(cJSON *root, t_client *client) {
    // GHashTable **online_users = mx_get_online_users();
    // gint64 user_id = (client->uid == 1) ? 2 : 1;


    // g_hash_table_insert(*online_users, &(client->uid), client);
    // // g_hash_table_foreach(*online_users, print_hash_table, NULL);
    // if (request_count > 1) {
        // GDataOutputStream *data_out = mx_get_socket_by_user_id(user_id);

        // if (data_out)
            // mx_send_data(data_out, data);
    // }
    // g_print("sign_up input: %s\n",data);
    // (void)client;
// }

void (*const request_handler[])() = {
    mx_sign_up
};

void get_data(GObject *source_object, GAsyncResult *res, gpointer socket) {
    t_client *new_client = (t_client*)socket;
    GError *error = NULL;
    gchar *data = NULL;

    if (!g_socket_connection_is_connected(new_client->connection)) {
        g_print("Client logout!\n");
        return;
    }
    data = g_data_input_stream_read_line_finish(new_client->data_in,
                                                res, NULL, &error);
    g_print("input data: %s\n", data);
    if (data) {
        cJSON *root = cJSON_Parse(data);
        gint req_type = cJSON_GetObjectItem(root, "request_type")->valueint;

        request_handler[req_type](root, new_client);
        g_free(data);
        cJSON_Delete(root);
    }
    if (error) {
        g_error("%s\n", error->message);
        g_clear_error(&error);
    }
    g_data_input_stream_read_line_async(new_client->data_in, G_PRIORITY_DEFAULT, NULL, get_data, new_client);
    (void)source_object;
}


/* this function will get called everytime a client attempts to connect */
gboolean incoming_callback(GSocketService *service,
                           GSocketConnection *connection,
                           GObject *source_object,
                           gpointer user_data) {
    GInputStream *istream = g_io_stream_get_input_stream(G_IO_STREAM(connection));
    GOutputStream *ostream = g_io_stream_get_output_stream(G_IO_STREAM(connection));
    GDataInputStream *data_in = g_data_input_stream_new(istream);
    GDataOutputStream *data_out = g_data_output_stream_new(ostream);
    t_client *socket = g_malloc(sizeof(t_client));

    g_print("Client connected!\n");

    socket->istream = g_object_ref(istream);
    socket->ostream = g_object_ref(ostream);
    socket->data_in = g_object_ref(data_in);
    socket->data_out = g_object_ref(data_out);
    socket->connection = g_object_ref(connection);
    socket->uid = gui++;

    g_data_input_stream_read_line_async(socket->data_in, G_PRIORITY_DEFAULT, NULL, get_data, socket);
    (void)service;
    (void)source_object;
    return FALSE;
}

void mx_dest_client(gpointer data) {
    t_client *socket = (t_client*)data;

    g_object_unref(socket->istream);
    g_object_unref(socket->ostream);
    g_object_unref(socket->data_in);
    g_object_unref(socket->data_out);
    g_object_unref(socket->connection);
    g_free(socket);
}

int main(int argc, char **argv) {
    GSocketService *service = NULL;
    GMainLoop *loop = NULL;
    GError *error = NULL;
    GHashTable **online_users = mx_get_online_users();

    *online_users = g_hash_table_new_full(g_int_hash, NULL,
                                          NULL, NULL);
    // database variable
    char *errmsg = NULL;
    sqlite3 **db = mx_get_db();
    int rc = 0;

    // database setup start
    if ((rc = sqlite3_open("uchat_db", db)) != SQLITE_OK) {
            g_error("Can't open database: %s\n", sqlite3_errmsg(*db));
            sqlite3_close(*db);
    }
    if ((rc = sqlite3_exec(*db, "SELECT User_id FROM users_credential LIMIT 1;",
                    NULL, NULL, &errmsg)) != SQLITE_OK) {
        if ((rc = sqlite3_exec(*db, "CREATE TABLE users_credential ( \
                            user_id INTEGER NOT NULL, \
                            login VARCHAR(50) NOT NULL, \
                            passwd_hash VARCHAR(32) NOT NULL, \
                            PRIMARY KEY(user_id));", NULL, NULL, &errmsg)) != SQLITE_OK) {
                g_error("Failed to create table: %s\n", errmsg);
                sqlite3_close(*db);
        }
    }
    service = g_socket_service_new();
    g_socket_listener_add_inet_port((GSocketListener*)service, 5050, NULL, &error);
    g_signal_connect(service, "incoming", G_CALLBACK(incoming_callback), NULL);

    g_print("Waiting for client!\n");

    loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(loop);
    (void)argc;
    (void)argv;
    return 0;
}
