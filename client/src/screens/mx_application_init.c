#include "client.h"

int mx_application_run(int argc, char **argv, GtkApplication *app) {
    int status;

    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}

static void activate(GtkApplication *app, t_client *client) {
    client->phone_entering = mx_phone_entering_window(client);
    client->password_validation = mx_password_validate_window(client);
    client->profile_setuping = mx_profile_setuping_window(client);

    gtk_application_add_window(app, client->phone_entering);
    gtk_application_add_window(app, client->password_validation);
    gtk_application_add_window(app, client->profile_setuping);
    gtk_widget_show(GTK_WIDGET(client->phone_entering));
}

void mx_application_init(t_client *client) {
  client->app = gtk_application_new("org.gnome.chat.desktop",
                                    G_APPLICATION_FLAGS_NONE);
  g_signal_connect(client->app, "activate", G_CALLBACK(activate), client);
}