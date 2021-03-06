#include "server.h"
#define REQUEST_HANDLER_SIZE 13

void print_hash_table(gpointer key, gpointer value, gpointer user_data) {
    g_print("Connected user id is %lld\n", *(gint64 *)key);
    (void)user_data;
    (void)value;
}

sqlite3 **mx_get_db(void) {
    static sqlite3 *db = NULL;

    return &db;
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

void (*const request_handler[REQUEST_HANDLER_SIZE])() = {
    mx_password_request_handler,
    mx_auth_request_handler,
    mx_sign_up_request_handler,
    mx_get_contact_handler,
    mx_add_contact_handler,
    mx_get_contacts_list,
    mx_send_message,
    mx_get_chat_history,
    mx_sign_out_request_handler,
    mx_remove_contact_handler,
    mx_delete_chat,
    mx_get_profile_info,
    mx_change_profile,
};

gint mx_free_client(t_client *client) {
    GHashTable *online_users = *(mx_get_online_users());

    if (client->uid == 0) {
        mx_dest_client(client);
        return 1;
    }
    if (g_hash_table_remove(online_users,
                            GINT_TO_POINTER(client->uid)) == TRUE) {
        mx_dest_client(client);
    }
    else {
        g_warning("Failed to delete user from hash table!");
        return -1;
    }
    return 1;
}

static gboolean json_validator(const cJSON *root, t_client *client) {
    cJSON *req_type = cJSON_GetObjectItemCaseSensitive(root, "request_type");

    if (!root || !req_type || !cJSON_IsNumber(req_type))
        return FALSE;
    if (req_type->valueint > REQUEST_HANDLER_SIZE)
        return FALSE;
    if (req_type->valueint > RQ_SIGN_UP) {
        cJSON *token = cJSON_GetObjectItemCaseSensitive(root, "token");

        if (token && cJSON_IsString(token)
            && !g_strcmp0(client->token, token->valuestring))
            return TRUE;
        else
            return FALSE;
    }
    return TRUE;
}


void get_data(GObject *source_object, GAsyncResult *res, gpointer socket) {
    t_client *new_client = (t_client*)socket;
    GError *error = NULL;
    gchar *data = NULL;

    data = g_data_input_stream_read_line_finish(new_client->data_in,
                                                res, NULL, &error);
    if (data) {
        g_message("data from client - %s\n", data);
        cJSON *root = cJSON_Parse(data);
        if (json_validator(root, new_client)) {
            cJSON *req_type = cJSON_GetObjectItem(root, "request_type");

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
        mx_free_client(new_client);
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

void mx_daemon(void) {
    pid_t process_id = 0;
    pid_t sid = 0;

    process_id = fork();
    if (process_id < 0) {
        g_printerr("fork failed!\n");
        exit(1);
    }
    if (process_id > 0) {
        g_print("process_id of child process %d \n", process_id);
        exit(0);
    }
    umask(0);
    sid = setsid();
    if (sid < 0)
        exit(1);
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}

int main(int argc, char **argv) {
    GSocketService *service = NULL;
    GMainLoop *loop = NULL;
    GError *error = NULL;
    GHashTable **online_users = mx_get_online_users();

    *online_users = g_hash_table_new(NULL, NULL);
    service = g_socket_service_new();

    mx_db_init();

    g_socket_listener_add_inet_port((GSocketListener*)service, 5050, NULL, &error);
    g_signal_connect(service, "incoming", G_CALLBACK(incoming_callback), NULL);

    g_thread_new("message sheduler", mx_message_sheduler, *(mx_get_db()));

    mx_daemon();
    loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(loop);
    (void)argc;
    (void)argv;
    return 0;
}
