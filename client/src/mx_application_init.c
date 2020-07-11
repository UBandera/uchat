#include "client.h"

int mx_application_run(int argc, char **argv, GApplication *app) {
    int status;

    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}

GApplication *mx_application_init(gpointer user_data) {
  GApplication *app = g_application_new("org.gnome.chat.desktop", G_APPLICATION_HANDLES_COMMAND_LINE);
  GError *error = NULL;

  g_application_register(app, NULL, &error);
  // mx_notify(app);
  g_application_activate(app);
  // TODO
  // g_signal_connect(app, "activate", G_CALLBACK(activate), user_data);

  return app;
}
