#ifndef CLIENT_H
#define CLIENT_H

#include <gio/gio.h>
#include <gtk/gtk.h>
#include <string.h>
#include "mx_json.h"

enum e_auth_data_validation {
    MX_VALID = 0,
    MX_LOGIN_ERROR,
    MX_PASSWORD_ERROR,
    MX_REPEAT_PASSWORD_ERROR,
}

#define MX_FORBIDEN_PATTERN "[^A-Z&^a-z&^0-9&^(!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~)]"
#define MX_ALLOWED_PATTERN "^(?=.*?[A-Z])(?=.*?[a-z])(?=.*?[0-9])(?=.*?[!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~]).{6,48}$"

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

// requests
gchar *mx_form_auth_request(gchar *login, gchar *password, gint type);
gchar *mx_form_contact_list_request(void);
gchar *mx_form_chat_data_request(gchar *user_id);
gchar *mx_form_send_message_request(gchar *user_id, gchar *message);

gssize mx_send_data(GDataOutputStream *data_out, gchar *data);
int login(int argc, char **argv, gpointer user_data); // for testing


gboolean mx_match(const gchar *str, const gchar *pattern, gint compile_flags, gint match_flag);
gint mx_auth_confirming(gchar *login, gchar *password, gchar *repeat_password)

#endif /* end of include guard: CLIENT_H */
