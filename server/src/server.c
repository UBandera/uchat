#include "server.h"
#define REQUEST_HANDLER_SIZE 8

void print_hash_table(gpointer key, gpointer value, gpointer user_data) {
    g_print("Connected user id is %lld\n", *(gint64 *)key);
    (void)user_data;
    (void)value;
}

sqlite3 **mx_get_db(void) {
    static sqlite3 *db = NULL;

    return &db;
}

void (*const request_handler[REQUEST_HANDLER_SIZE])() = {
    mx_password_request_handler,
    mx_auth_request_handler,
    mx_sign_up_request_handler,
    mx_get_contact_handler,
    mx_add_contact_handler,
    mx_get_contacts_list,
    mx_send_message,
    mx_get_chat_history
};

void get_data(GObject *source_object, GAsyncResult *res, gpointer socket) {
    t_client *new_client = (t_client*)socket;
    GError *error = NULL;
    gchar *data = NULL;

    data = g_data_input_stream_read_line_finish(new_client->data_in,
                                                res, NULL, &error);
        g_print("input data: %s\n", data);
    if (data) {
        cJSON *root = cJSON_Parse(data);
        cJSON *req_type = cJSON_GetObjectItem(root, "request_type");

        if (root != NULL && req_type != NULL
            && req_type->valueint < REQUEST_HANDLER_SIZE) {
            request_handler[req_type->valueint](root, new_client);
            g_free(data);
            cJSON_Delete(root);
        }
        else
            g_warning("Invalid request, try again!\n");
    }
    else if (error) {
        g_error("%s\n", error->message);
        g_clear_error(&error);
    }
    else {
        g_print("Client logout!\n");
        return;
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
    socket->uid = 0;

    g_data_input_stream_read_line_async(socket->data_in, G_PRIORITY_DEFAULT, NULL, get_data, socket);
    (void)service;
    (void)source_object;
    (void)user_data;
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

    *online_users = g_hash_table_new_full(g_int_hash, NULL, NULL, NULL);
    service = g_socket_service_new();

    mx_db_init();

    g_socket_listener_add_inet_port((GSocketListener*)service, 5050, NULL, &error);
    g_signal_connect(service, "incoming", G_CALLBACK(incoming_callback), NULL);

    g_print("Waiting for client!\n");

    loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(loop);
    (void)argc;
    (void)argv;
    return 0;
}
