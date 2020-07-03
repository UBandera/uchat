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

gssize mx_send_data(GDataOutputStream *data_out, gchar *data);
int login(int argc, char **argv, gpointer user_data);


#endif /* end of include guard: CLIENT_H */
