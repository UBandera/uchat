#ifndef SERVER_H
#define SERVER_H

#include <gio/gio.h>
#include <gtk/gtk.h>
#include <string.h>
#include "mx_json.h"
#include <sqlite3.h>
#include <stdbool.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <time.h>

typedef struct s_client {
    GSocketConnection *connection;
    GInputStream *istream;
    GOutputStream *ostream;
    GDataInputStream *data_in;
    GDataOutputStream *data_out;
    gint32 uid;

    void (*request_handler[RQ_RECOVERY_PASSWD + 1])();
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

//database
void mx_db_init(void);

//Sign_up_rq
gint mx_sign_up_quary(cJSON *root, sqlite3 *db);
gboolean mx_check_if_user_excist(cJSON *root, sqlite3 *db);
void mx_sign_up(cJSON *root, t_client *client);

//Sign_up_rq
gint get_user_id_run(sqlite3_stmt *stmt, t_client *client);
gint get_user_id_prepare(cJSON *root, sqlite3_stmt **stmt);
void mx_sign_in(cJSON *root, t_client *client);

void mx_recovery_password(cJSON *root, t_client *client);
char *mx_recovery_body(char *user_name);
int mx_send_mail(char *receiver, char *body);
char *mx_generate_password(void);

#endif /* end of include guard: SERVER_H */
