#include "mx_client.h"

void get_data(GObject *source_object, GAsyncResult *res, gpointer user_data) {
    t_client *client = (t_client *)user_data;
    GError *error = NULL;
    gsize size = 0;
    gchar *data = NULL;

    if (!g_socket_connection_is_connected(client->connection)) {
        g_print("Server logout!\n");
        return;
    }
    data = g_data_input_stream_read_line_finish(client->data_in, res, &size, &error);
    if (error) {
        g_error("%s\n", error->message);
        g_clear_error(&error);
    }
    if (data) {
        g_print("response = %s [from serv]\n", data);
        g_free(data);
        // return;
    }
    g_data_input_stream_read_line_async(client->data_in, G_PRIORITY_DEFAULT, NULL, get_data, client);
    (void)source_object;
}

t_client *init_client(GSocketConnection *connection) {
    t_client *client = g_new(t_client, 1);
    GInputStream *istream = g_io_stream_get_input_stream(G_IO_STREAM(connection));
    GOutputStream *ostream = g_io_stream_get_output_stream(G_IO_STREAM(connection));
    GDataInputStream *data_in = g_data_input_stream_new(istream);
    GDataOutputStream *data_out = g_data_output_stream_new(ostream);

    client->connection = g_object_ref(connection);
    client->istream = g_object_ref(istream);
    client->ostream = g_object_ref(ostream);
    client->data_in = g_object_ref(data_in);
    client->data_out = g_object_ref(data_out);
    g_data_input_stream_read_line_async(data_in, G_PRIORITY_DEFAULT, NULL, get_data, client);
    return client;
}


int main(int argc, char **argv) {
    GSocketClient *client = NULL;
    GSocketConnection *connection = NULL;
    GError *error = NULL;
    t_client *client_st = NULL;

    client = g_socket_client_new();

    // some settings
    g_socket_client_set_protocol(client, G_SOCKET_PROTOCOL_TCP);
    g_socket_client_set_socket_type(client, G_SOCKET_TYPE_STREAM);
    // g_socket_client_set_enable_proxy(client, TRUE); // Future release

    connection = g_socket_client_connect_to_host(client, (gchar *)"0.0.0.0", 5050, NULL, &error);

    if (error) {
        g_error("%s\n", error->message);
        g_clear_error(&error);
    }
    client_st = init_client(connection);

    // mx_form_login_request("admin", "12345678", client_st);

    // ui (for testing)
    // mx_application_run(argc, argv, mx_application_init(client_st));
    login(argc, argv, client_st);
    g_object_unref(connection);
    g_free(client_st);
    return 0;
}
