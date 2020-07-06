#ifndef SERVER_H
#define SERVER_H

#include <gio/gio.h>
#include <gtk/gtk.h>
#include <string.h>
#include "mx_json.h"

typedef struct s_client {
    GSocketConnection *connection;
    GInputStream *istream;
    GOutputStream *ostream;
    GDataInputStream *data_in;
    GDataOutputStream *data_out;
    guint64 uid;
}              t_client;

/*
   contain connection from new client and hash table in which should be add
   uid as a key and streams of client as a value
*/
typedef struct s_new_client {
    GHashTable *online_users;
    t_client *client;
}              t_new_client;



GHashTable **mx_get_online_users(void);
GDataOutputStream *mx_get_socket_by_user_id(gint64 user_id);
gssize mx_send_data(GDataOutputStream *data_out, gchar *data);


#endif /* end of include guard: SERVER_H */
