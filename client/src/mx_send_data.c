#include <gio/gio.h>

gssize mx_send_data(GDataOutputStream *data_out, gchar *data) {
    GError *error = NULL;
    gsize size = 0;

    if (!g_data_output_stream_put_string(data_out, data, NULL, &error)) {
        g_print("Write string error\n");
    }
    if (error) {
        g_error("%s\n", error->message);
        g_clear_error(&error);
    }
    return size;
}
