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
};

#define MX_FORBIDEN_PATTERN "[^A-Z&^a-z&^0-9& \
                             ^(!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~)]"
#define MX_ALLOWED_PATTERN "^(?=.*?[A-Z])(?=.*?[a-z])(?=.*?[0-9]) \
                            (?=.*?[!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~]). \
                            {6,48}$"

typedef struct s_client {
    GSocketConnection *connection;
    GInputStream *istream;
    GOutputStream *ostream;
    GDataInputStream *data_in;
    GDataOutputStream *data_out;
    GtkBuilder *builder;
    void (*response_handler[RQ_SEND_MESSAGE])(cJSON *json,
                                              struct s_client *client);
    // int (*routing[])(GtkBuiler *builder);
}              t_client;


// requests
gchar *mx_form_auth_request(gchar *login, gchar *password, gint type);
gchar *mx_form_contact_list_request(void);
gchar *mx_form_chat_data_request(gint user_id);
gchar *mx_form_profile_data_request(void);
gchar *mx_form_send_message_request(gint user_id, gchar *message);
gchar *mx_form_signout_request(void);

void mx_init_handlers(t_client *client);

// responses
void mx_sign_up_response(cJSON *json, t_client *client);
void mx_sign_in_response(cJSON *json, t_client *client);


gssize mx_send_data(GDataOutputStream *data_out, gchar *data);


// validation
gint mx_auth_confirming(gchar *login, gchar *password,
                        gchar *repeat_password);

// Utils
gboolean mx_match(const gchar *str, const gchar *pattern,
                  gint compile_flags, gint match_flag);

// Trash
int login(int argc, char **argv, gpointer user_data); // for testing
// int mx_application_run(int argc, char **argv, GApplication *app);
// GApplication *mx_application_init(gpointer user_data);
// void mx_notify(GApplication *application);

#endif /* end of include guard: CLIENT_H */
