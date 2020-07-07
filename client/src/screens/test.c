#include "mx_client.h"

#define MX_TITLE "uChat"
#define MX_WELCOME "Welcome to the macOS application"


typedef struct s_form {
    t_client *client;
    GtkWidget *window;
    GtkWidget *body;
    GtkWidget *header;
    GtkWidget *header_items;
    GtkWidget *form;
    GtkWidget *inputs;
    GtkWidget *floor;
    GtkWidget *login_input;
    GtkWidget *password_input;
    GtkWidget *image;
    GtkWidget *logo;
    GtkWidget *welcome_label;
    GtkButton *log_in_btn;
    GtkButton *sign_up_btn;
    GtkEntryBuffer *entry_buffer;
    GtkCssProvider *provider;
}               t_form;

// void (GObject *source_object, GAsyncResult *res, gpointer user_data);


void sign_in_req(GtkButton *button, t_form *data) {
    gchar *login = (gchar *)gtk_entry_get_text(GTK_ENTRY(((t_form *)data)->login_input));
    gchar *password = (gchar *)gtk_entry_get_text(GTK_ENTRY(((t_form *)data)->password_input));
    t_client *client = data->client;

    if (mx_auth_confirming(login, password, NULL) == MX_VALID) {
        gchar *request = mx_form_auth_request(login, password, RQ_SIGN_UP);
        mx_send_data(client->data_out, request);
    }
    (void)button;
}

void sign_up_req(GtkButton *button, t_form *data) {
    gchar *login = (gchar *)gtk_entry_get_text(GTK_ENTRY(((t_form *)data)->login_input));
    gchar *password = (gchar *)gtk_entry_get_text(GTK_ENTRY(((t_form *)data)->password_input));
    t_client *client = data->client;

    if (mx_auth_confirming(login, password, NULL) == MX_VALID) {
        gchar *request = mx_form_auth_request(login, password, RQ_SIGN_UP);
        mx_send_data(client->data_out, request);
    }
    (void)button;
}

int login(int argc, char **argv, gpointer user_data) {
    t_form *form_template = (t_form *)malloc(sizeof(t_form));

    gtk_init(&argc, &argv);
    form_template->client = (t_client *)user_data;
    form_template->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    form_template->body = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
    form_template->login_input = gtk_entry_new();
    form_template->password_input = gtk_entry_new();
    form_template->log_in_btn = GTK_BUTTON(gtk_button_new_with_label("SIGN IN"));
    form_template->sign_up_btn = GTK_BUTTON(gtk_button_new_with_label("SIGN UP"));

    gtk_container_add(GTK_CONTAINER(form_template->window), form_template->body);

    gtk_container_add(GTK_CONTAINER(form_template->body), form_template->login_input);
    gtk_container_add(GTK_CONTAINER(form_template->body), form_template->password_input);
    gtk_container_add(GTK_CONTAINER(form_template->body), GTK_WIDGET(form_template->log_in_btn));
    gtk_container_add(GTK_CONTAINER(form_template->body), GTK_WIDGET(form_template->sign_up_btn));

    g_signal_connect(form_template->log_in_btn, "clicked", G_CALLBACK(sign_in_req), form_template);
    g_signal_connect(form_template->sign_up_btn, "clicked", G_CALLBACK(sign_up_req), form_template);

    g_signal_connect(form_template->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(form_template->window);
    gtk_main();
    return 0;
}
