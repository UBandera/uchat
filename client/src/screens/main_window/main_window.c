#include "client.h"

#define MX_MAIN_WINDOW "./src/screens/glade/main.glade"

GtkWindow *mx_main_window(t_client *client) {
    GtkBuilder *builder = client->builder;
    GError *error = NULL;
    GtkWindow *window = NULL;

    // mx_apply_styles(MX_STYLES);
    if (!gtk_builder_add_from_file(builder,
                                   MX_MAIN_WINDOW,
                                   &error))
        g_error("%s\n", error->message);
    window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));
    return window;
}
