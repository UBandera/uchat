#include "mx_server.h"

void request_handling(gchar *data, t_server *server) {
    mx_print_json(data);
    (void)server;
}

void get_data(GObject *source_object, GAsyncResult *res, gpointer user_data) {
    t_server *server = (t_server *)user_data;
    GError *error = NULL;
    gsize size = 0;
    gchar *data = NULL;

    if (!g_socket_connection_is_connected(server->connection)) {
        g_print("Client logout!\n");
        return;
    }
    data = g_data_input_stream_read_line_finish(server->data_in, res, &size, &error);
    if (data) {
        request_handling(data, server);
        g_free(data);
        // return;
    }
    if (error) {
        g_error("%s\n", error->message);
        g_clear_error(&error);
    }
    g_data_input_stream_read_line_async(server->data_in, G_PRIORITY_DEFAULT, NULL, get_data, server);
    (void)source_object;
}

/* this function will get called everytime a client attempts to connect */
gboolean incoming_callback(GSocketService *service,
                           GSocketConnection *connection,
                           GObject *source_object,
                           gpointer user_data) {
    GInputStream * istream = g_io_stream_get_input_stream(G_IO_STREAM(connection));
    GOutputStream * ostream = g_io_stream_get_output_stream(G_IO_STREAM(connection));
    GDataInputStream *data_in = g_data_input_stream_new(istream);
    GDataOutputStream *data_out = g_data_output_stream_new(ostream);
    t_server *server = g_malloc(sizeof(t_server));

    g_print("Client connected!\n");

    server->istream = g_object_ref(istream);
    server->ostream = g_object_ref(ostream);
    server->data_in = g_object_ref(data_in);
    server->data_out = g_object_ref(data_out);
    server->connection = g_object_ref(connection);
    g_data_input_stream_read_line_async(server->data_in, G_PRIORITY_DEFAULT, NULL, get_data, server);
    (void)service;
    (void)source_object;
    (void)user_data;
    return FALSE;
}

int main(int argc, char **argv) {
    GSocketService *service = NULL;
    GMainLoop *loop = NULL;
    GError * error = NULL;
    // database variable
    char *errmsg = NULL;
    sqlite3 *db;
    int rc = 0;

    // database setup start
    if ((rc = sqlite3_open("uchat_db", &db)) != SQLITE_OK) {
            g_error("Can't open database: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
    }
    if ((rc = sqlite3_exec(db, "SELECT User_id FROM users_credential LIMIT 1;",
                    NULL, NULL, &errmsg)) != SQLITE_OK) {
        if ((rc = sqlite3_exec(db, "CREATE TABLE users_credential ( \
                            user_id INT NOT NULL, \
                            login VARCHAR(50) NOT NULL, \
                            passwd_hash VARCHAR(32) NOT NULL, \
                            PRIMARY KEY(user_id, login));", NULL, NULL, &errmsg)) != SQLITE_OK) {
                g_error("Failed to create table: %s\n", errmsg);
                sqlite3_close(db);
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
