#ifndef SERVER_H
#define SERVER_H

#include <gio/gio.h>
#include <gtk/gtk.h>
#include <string.h>
#include "mx_json.h"
#include <sqlite3.h>
#include <stdbool.h>

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
sqlite3 **mx_get_db(void); 

GDataOutputStream *mx_get_socket_by_user_id(gint64 user_id);
gssize mx_send_data(GDataOutputStream *data_out, gchar *data);

//Sign_up_rq
gint mx_sign_up_quary(cJSON *root, sqlite3 *db);
gboolean mx_check_if_user_excist(cJSON *root, sqlite3 *db);
void mx_sign_up(cJSON *root, t_client *client);

#endif /* end of include guard: SERVER_H */
