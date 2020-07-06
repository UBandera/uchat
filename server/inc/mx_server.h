#ifndef SERVER_H
#define SERVER_H

#include <gio/gio.h>
#include <gtk/gtk.h>
#include <string.h>
#include "mx_json.h"

typedef struct s_server {
    GSocketConnection *connection;
    GInputStream *istream;
    GOutputStream *ostream;
    GDataInputStream *data_in;
    GDataOutputStream *data_out;
}              t_server;

gssize mx_send_data(GDataOutputStream *data_out, gchar *data);


#endif /* end of include guard: SERVER_H */
