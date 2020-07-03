#ifndef CLIENT_H
#define CLIENT_H

#include <gio/gio.h>
#include <gtk/gtk.h>
#include <string.h>


typedef struct s_client {
    GSocketConnection *connection;
    GInputStream *istream;
    GOutputStream *ostream;
    GDataInputStream *data_in;
    GDataOutputStream *data_out;
}              t_client;

int mx_application_run(int argc, char **argv, GApplication *app);
GApplication *mx_application_init(gpointer user_data);
void mx_notify(GApplication *application);
void mx_form_login_request(gchar *login, gchar *password, t_client *client);

gssize mx_send_data(GDataOutputStream *data_out, gchar *data);
int login(int argc, char **argv, gpointer user_data); // for testing

#endif /* end of include guard: CLIENT_H */
