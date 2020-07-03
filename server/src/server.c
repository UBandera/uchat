#include "mx_server.h"
const gboolean NEW_USER = 0;
gint64 gui = 1;

void request_handling(gchar *data, t_client *client) {
    g_print("%s\n",data);
    (void)client;
}

void get_data(GObject *source_object, GAsyncResult *res, gpointer user_data) {
    t_new_client *new_client = (t_new_client*)user_data;
    t_client *client = new_client->client;
    GHashTable *online_users = new_client->online_users;
    GError *error = NULL;
    gsize size = 0;
    gchar *data = NULL;
    gint64 uid = gui++;
    

    if (!g_socket_connection_is_connected(client->connection)) {
        g_print("Client logout!\n");
        return;
    }
    data = g_data_input_stream_read_line_finish(client->data_in, res, &size, &error);
    g_hash_table_insert(online_users, &(uid), client);
    if (data) {
        request_handling(data, client);
        g_free(data);
    }
    if (error) {
        g_error("%s\n", error->message);
        g_clear_error(&error);
    }
    g_data_input_stream_read_line_async(client->data_in, G_PRIORITY_DEFAULT, NULL, get_data, new_client);
    (void)source_object;
}

void print_hash_table(gpointer key, gpointer value, gpointer user_data) {
    g_print("%lld\n",(gint64)key);
}

/* this function will get called everytime a client attempts to connect */
gboolean incoming_callback(GSocketService *service,
                           GSocketConnection *connection,
                           GObject *source_object,
                           gpointer online_users) {
    GInputStream *istream = g_io_stream_get_input_stream(G_IO_STREAM(connection));
    GOutputStream *ostream = g_io_stream_get_output_stream(G_IO_STREAM(connection));
    GDataInputStream *data_in = g_data_input_stream_new(istream);
    GDataOutputStream *data_out = g_data_output_stream_new(ostream);
    t_client *socket = g_malloc(sizeof(t_client));
    t_new_client *new_client = g_malloc(sizeof(t_new_client));

    g_print("Client connected!\n");

    socket->istream = g_object_ref(istream);
    socket->ostream = g_object_ref(ostream);
    socket->data_in = g_object_ref(data_in);
    socket->data_out = g_object_ref(data_out);
    socket->connection = g_object_ref(connection);
    new_client->client = socket;
    new_client->online_users = (GHashTable *)online_users;
    g_data_input_stream_read_line_async(socket->data_in, G_PRIORITY_DEFAULT, NULL, get_data, new_client);
    g_hash_table_foreach(online_users, print_hash_table, NULL);
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
    GHashTable *online_users = g_hash_table_new_full(g_int64_hash, NULL,
                                                     NULL, mx_dest_client);

    service = g_socket_service_new();
    g_socket_listener_add_inet_port((GSocketListener*)service, 5050, NULL, &error);
    g_signal_connect(service, "incoming", G_CALLBACK(incoming_callback), online_users);

    g_print("Waiting for client!\n");

    loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(loop);
    (void)argc;
    (void)argv;
    return 0;
}
