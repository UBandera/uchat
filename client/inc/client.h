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

#define MX_FORBIDEN_PATTERN "[^A-Z&^a-z&^0-9&^(!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~)]"
#define MX_ALLOWED_PATTERN "^(?=.*?[A-Z])(?=.*?[a-z])(?=.*?[0-9])(?=.*?[!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~]).{6,48}$"

typedef struct s_client {
    GSocketConnection *connection;
    GDataInputStream *data_in;
    GDataOutputStream *data_out;
    gchar *token;

    GtkApplication *app;
    GtkBuilder *builder;
    GtkWindow *phone_entering;
    GtkWindow *password_validation;
    GtkWindow *profile_setuping;
    GtkWindow *main_window;

    void (*response_handler[30])(cJSON *json,
                                                  struct s_client *client);
}              t_client;


// screens
void mx_apply_styles(const gchar *path_to_css);
void mx_window_switcher(GtkWindow *hide, GtkWindow *show);
GtkWindow *mx_phone_entering_window(t_client *client);
GtkWindow *mx_password_validate_window(t_client *client);
GtkWindow *mx_profile_setuping_window(t_client *client);
GtkWindow *mx_main_window(t_client *client);


void mx_edit_login(GtkEntry *entry, GtkEntryIconPosition icon_pos,
                   GdkEvent *event, t_client *client);


// requests
gchar *mx_password_request(const gchar *phone);
gchar *mx_auth_request(const gchar *phone, const gchar *password);
gchar *mx_set_up_profile_request(const gchar *phone,
                                 const gchar *first_name,
                                 const gchar *last_name,
                                 const gchar *email);
gchar *mx_contacts_request(const gchar *token);

gchar *mx_form_auth_request(gchar *login, gchar *password, gint type);
gchar *mx_form_contact_list_request(void);
gchar *mx_form_chat_data_request(gint user_id);
gchar *mx_form_profile_data_request(void);
gchar *mx_form_send_message_request(gint user_id, gchar *message);
gchar *mx_form_signout_request(void);
gchar *mx_form_recovery_password_request(gchar *login);

void mx_receive_data(gchar *response, t_client *client);
void mx_init_handlers(t_client *client);

// responses
void mx_handle_password(cJSON *json, t_client *client);
void mx_sign_up_user(cJSON *json, t_client *client);
void mx_auth_validated(cJSON *json, t_client *client);
void mx_invalid_password_handler(cJSON *json, t_client *client);
void mx_sms_error_handler(cJSON *json, t_client *client);


void mx_sign_out_response(cJSON *json, t_client *client);
void mx_chat_data_response(cJSON *json, t_client *client);
void mx_contact_list_response(cJSON *json, t_client *client);
void mx_profile_data_response(cJSON *json, t_client *client);
void mx_send_message_response(cJSON *json, t_client *client);

gssize mx_send_data(GDataOutputStream *data_out, gchar *data);

// validation
gint mx_auth_confirming(gchar *login, gchar *password,
                        gchar *repeat_password);

// Utils
gboolean mx_match(const gchar *str, const gchar *pattern,
                  gint compile_flags, gint match_flag);
void get_message_data_from_json(cJSON *json, t_client *client);

// Trash
int mx_application_run(int argc, char **argv, GtkApplication *app);
void mx_application_init(t_client *client);
// void mx_notify(GApplication *application);

#endif /* end of include guard: CLIENT_H */
