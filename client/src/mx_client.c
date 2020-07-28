#include "client.h"

void get_data(GObject *source_object, GAsyncResult *res, gpointer user_data) {
    t_client *client = (t_client *)user_data;
    GError *error = NULL;
    gsize size = 0;
    gchar *data = NULL;

    data = g_data_input_stream_read_line_finish(client->data_in, res, &size, &error);
    if (error) {
        g_error("%s\n", error->message);
        g_clear_error(&error);
    }
    if (data) {
        mx_receive_data(data, client);
    }
    g_data_input_stream_read_line_async(client->data_in, G_PRIORITY_DEFAULT, NULL, get_data, client);
    (void)source_object;
}

t_client *init_client(GSocketConnection *connection) {
    t_client *client = g_new(t_client, 1);
    GInputStream *istream = NULL;
    GOutputStream *ostream = NULL;

    istream = g_io_stream_get_input_stream(G_IO_STREAM(connection));
    ostream = g_io_stream_get_output_stream(G_IO_STREAM(connection));

    client->connection = g_object_ref(connection);
    client->data_in = g_data_input_stream_new(istream);
    client->data_out = g_data_output_stream_new(ostream);
    client->builder = gtk_builder_new();
    client->token = NULL;
    client->contacts_table = NULL;
    mx_application_init(client);
    mx_init_handlers(client);
    g_data_input_stream_read_line_async(client->data_in,
                                        G_PRIORITY_DEFAULT, NULL,
                                        get_data, client);
    return client;
}

int main(int argc, char **argv) {
    GSocketClient *socket = NULL;
    GSocketConnection *connection = NULL;
    GError *error = NULL;
    t_client **client = mx_get_client();

    socket = g_socket_client_new();
    g_socket_client_set_protocol(socket, G_SOCKET_PROTOCOL_TCP);
    g_socket_client_set_socket_type(socket, G_SOCKET_TYPE_STREAM);
    connection = g_socket_client_connect_to_host(socket, (gchar *)"0.0.0.0",
                                                 5050, NULL, &error);
    if (error) {
        g_error("%s\n", error->message);
        g_clear_error(&error);
    }
    *client = init_client(connection);
    mx_application_run(argc, argv, (*client)->app);

    g_object_unref(connection);
    g_free(*client);
    return 0;
}
