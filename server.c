#include <gio/gio.h>
#include <glib.h>
#include <glib-object.h>

#define MAX_THREADS 10
const gchar *ADDRESS = "0.0.0.0";
const guint16 PORT = 12345;

int main() {
    GSocketService *service = g_threaded_socket_service_new(MAX_THREADS);
    GSocketListener *listner = g_socket_listener_new();
    GInetAddress *Iaddress = g_inet_address_new_from_string(ADDRESS);
    GError *err = NULL;
    if (Iaddress == NULL)
        g_error("could not parse address %s\n", ADDRESS);
    GSocketAddress *address = g_inet_socket_address_new(ADDRESS, PORT);

    if (g_socket_listener_add_address(listner, address, G_SOCKET_TYPE_STREAM,
                                      G_SOCKET_PROTOCOL_TCP, NULL, NULL, err) == false) {
        g_error(err);
    }
    user_function(service, listner)

    // Free calls;
    g_object_unref(Iaddress);
}
