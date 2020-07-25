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

typedef struct s_contact_data {
    GtkWidget *row;
    gchar *first_name;
    gchar *last_name;
}              t_contact_data;


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
    GtkWindow *add_contact_dialog;

    GtkListBox *contacts;
    GtkListBox *chat;
    GtkWidget *chat_box;
    GtkWidget *contact_view;
    GtkWidget *contact_info;

    GHashTable *contacts_table;

    void (*response_handler[30])(cJSON *json, struct s_client *client);
}              t_client;


t_client **mx_get_client(void);
void mx_init_handlers(t_client *client);
void mx_receive_data(gchar *response, t_client *client);
gssize mx_send_data(GDataOutputStream *data_out, gchar *data);

// screens
void mx_apply_styles(const gchar *path_to_css);
void mx_window_switcher(GtkWindow *hide, GtkWindow *show);
GtkWindow *mx_phone_entering_window(t_client *client);
GtkWindow *mx_password_validate_window(t_client *client);
GtkWindow *mx_profile_setuping_window(t_client *client);
GtkWindow *mx_main_window(t_client *client);
GtkWindow *mx_add_contact_dialog(t_client *client);


void mx_show_contact_in_ui(t_client *client, gchar *first_name,
                           gchar *last_name, gint user_id);
gboolean mx_close_window_by_esc(GtkWidget *widget, GdkEventKey *event,
                                gpointer data);


// requests
gchar *mx_password_request(const gchar *phone);
gchar *mx_auth_request(const gchar *phone, const gchar *password);
gchar *mx_sign_up_request(const gchar *phone, const gchar *first_name,
                          const gchar *last_name, const gchar *email);
gchar *mx_find_contact_request(const gchar *phone, const gchar *token);
gchar *mx_add_contact_request(gint user_id, const gchar *token);
gchar *mx_contact_list_request(const gchar *token);


// responses
void mx_auth_validated(cJSON *json, t_client *client);
void mx_contact_list(cJSON *json, t_client *client);
void mx_contact_not_found(cJSON *json, t_client *client);
void mx_get_contact(cJSON *json, t_client *client);
void mx_invalid_password_handler(cJSON *json, t_client *client);
void mx_handle_password(cJSON *json, t_client *client);
void mx_sms_error_handler(cJSON *json, t_client *client);
void mx_sign_up_user(cJSON *json, t_client *client);



// validation
gint mx_auth_confirming(gchar *login, gchar *password,
                        gchar *repeat_password);

// Utils
gboolean mx_match(const gchar *str, const gchar *pattern,
                  gint compile_flags, gint match_flag);

// Trash
int mx_application_run(int argc, char **argv, GtkApplication *app);
void mx_application_init(t_client *client);
// void mx_notify(GApplication *application);

#endif /* end of include guard: CLIENT_H */
