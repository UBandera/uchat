#include "client.h"

#define MX_MAIN_WINDOW "./src/screens/glade/contact_info.glade"

static void close_callback(GtkWidget *button, GtkWidget *widget) {
    gtk_widget_hide(widget);
    (void)button;
}

GtkWindow *mx_contact_info_window(t_client *client) {
    GError *error = NULL;
    GtkWindow *window = NULL;
    GtkWidget *close = NULL;

    // mx_apply_styles(MX_STYLES);
    if (!gtk_builder_add_from_file(client->builder, MX_MAIN_WINDOW, &error))
        g_error("%s\n", error->message);
    window = GTK_WINDOW(gtk_builder_get_object(client->builder, "window"));
    close = GTK_WIDGET(gtk_builder_get_object(client->builder, "close_btn"));
    g_signal_connect(window, "clicked", G_CALLBACK(close_callback), window);
    g_signal_connect(window, "key_press_event",
                     G_CALLBACK(mx_close_widget), window);
    return window;
}
