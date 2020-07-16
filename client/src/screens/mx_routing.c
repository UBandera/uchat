#include "client.h"

void mx_routing(int argc, char **argv, t_client *client) {
    gtk_init(&argc, &argv);

    mx_phone_entering_window(client);
    gtk_main();
}
