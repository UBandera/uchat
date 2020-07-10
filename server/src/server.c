#include "server.h"

void mx_dest_client(gpointer data) {
    t_client *socket = (t_client*)data;

    g_object_unref(socket->istream);
    g_object_unref(socket->ostream);
    g_object_unref(socket->data_in);
    g_object_unref(socket->data_out);
    g_object_unref(socket->connection);
    g_free(socket);
}

sqlite3 **mx_get_db(void) {
    static sqlite3 *db = NULL;

    return &db;
}

void (*const request_handler[])() = {
    mx_sign_in,
    mx_sign_up
    // mx_send_message
};

void get_data(GObject *source_object, GAsyncResult *res, gpointer socket) {
    t_client *new_client = (t_client*)socket;
    GError *error = NULL;
    gchar *data = NULL;

    if (g_input_stream_is_closed(new_client->istream)) {
        g_print("is is closed\n");
        return;
    }
    if (g_output_stream_is_closed(new_client->ostream)) {
        g_print("out is closed\n");
        return;
    }

    data = g_data_input_stream_read_line_finish(new_client->data_in,
                                                res, NULL, &error);
    g_print("input data: %d\n", new_client->uid);
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
    // else if (data == NULL && error == NULL) {
        // mx_dest_client(new_client);
        // return;
    // }
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
    socket->uid = 0;

    g_data_input_stream_read_line_async(socket->data_in, G_PRIORITY_DEFAULT, NULL, get_data, socket);
    (void)service;
    (void)source_object;
    (void)user_data;
    return FALSE;
}

void check_if_user_online(gpointer key, gpointer value, gpointer user_data) {
    t_client *client = (t_client*)value;

    if (g_socket_connection_is_connected(client->connection) == false) {
        g_print("Client logout!\n");
        mx_dest_client(client);
    }
    (void)key;
    (void)user_data;
}

gint64 mx_generate_chat_id(gint32 uid1, gint32 uid2) {
    gint64 chat_id;
    gint32 bocal;

    if (uid1 < uid2) {
        bocal = uid1;
        uid1 = uid2;
        uid2 = bocal;
    }
    chat_id = uid1;
    chat_id = chat_id << 32;
    chat_id += (gint64)uid2;

    // if need to get uids from chat_id
    // gint64 left = 0xffffffff00000000;
    // gint64 right = 0x00000000ffffffff;
    // right = chat_id & right;
    // left = (chat_id & left) >> 32;
    // g_print("right = %lld left = %lld\n", right, left);
    return chat_id;
}

int main(int argc, char **argv) {
    GSocketService *service = NULL;
    GMainLoop *loop = NULL;
    GError *error = NULL;
    GHashTable **online_users = mx_get_online_users();

    *online_users = g_hash_table_new_full(g_int_hash, NULL, NULL, NULL);
    service = g_socket_service_new();

    mx_db_init();

    g_socket_listener_add_inet_port((GSocketListener*)service, 5050, NULL, &error);
    g_signal_connect(service, "incoming", G_CALLBACK(incoming_callback), NULL);

    g_print("chat_id = %lld\n", mx_generate_chat_id(14, 1));
    g_print("chat_id = %lld\n", mx_generate_chat_id(1, 14));
    g_print("Waiting for client!\n");

    // g_hash_table_foreach(*online_users, check_if_user_online, NULL);

    loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(loop);
    (void)argc;
    (void)argv;
    return 0;
}
