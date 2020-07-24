#include "client.h"

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
    GtkWidget *user_id;
    GtkWidget *chat_id;
    GtkWidget *image;
    GtkWidget *logo;
    GtkWidget *welcome_label;
    GtkWidget *message;
    GtkButton *log_in_btn;
    GtkButton *sign_up_btn;
    GtkButton *send_message;
    GtkEntryBuffer *entry_buffer;
    GtkCssProvider *provider;
}               t_form;

void sign_in_req(GtkButton *button, t_form *data) {
    gchar *login = (gchar *)gtk_entry_get_text(GTK_ENTRY(((t_form *)data)->login_input));
    gchar *password = (gchar *)gtk_entry_get_text(GTK_ENTRY(((t_form *)data)->password_input));
    t_client *client = data->client;
    // gtk_widget_hide(data->message);
    if (mx_auth_confirming(login, password, NULL) == MX_VALID) {
        gchar *request = mx_form_auth_request(login, password, RQ_SIGN_IN);
        mx_send_data(client->data_out, request);
    }
    (void)button;
}

void sign_up_req(GtkButton *button, t_form *data) {
    gchar *login = (gchar *)gtk_entry_get_text(GTK_ENTRY(((t_form *)data)->login_input));
    gchar *password = (gchar *)gtk_entry_get_text(GTK_ENTRY(((t_form *)data)->password_input));
    t_client *client = data->client;
    // gtk_widget_show(data->message);
    if (mx_auth_confirming(login, password, NULL) == MX_VALID) {
        gchar *request = mx_form_auth_request(login, password, RQ_SIGN_UP);
        mx_send_data(client->data_out, request);
    }
    (void)button;
}

void send_message_req(GtkButton *button, t_form *data) {
    gchar *message = (gchar *)gtk_entry_get_text(GTK_ENTRY(((t_form *)data)->message));
    gchar *user_id = (gchar *)gtk_entry_get_text(GTK_ENTRY(((t_form *)data)->user_id));
    gchar *chat_id = (gchar *)gtk_entry_get_text(GTK_ENTRY(((t_form *)data)->chat_id));
    t_client *client = data->client;
    // change this request
    gchar *request = mx_form_chat_data_request(g_ascii_strtoll(chat_id,
                                                               NULL, 10),
                                               0, 20);
    // gchar *request = mx_form_send_message_request(g_ascii_strtoll(user_id,
                                                                  // NULL, 10),
                                                  // message);
    mx_send_data(client->data_out, request);
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
    form_template->user_id = gtk_entry_new();
    form_template->chat_id = gtk_entry_new();
    form_template->message = gtk_entry_new();
    form_template->log_in_btn = GTK_BUTTON(gtk_button_new_with_label("SIGN IN"));
    form_template->sign_up_btn = GTK_BUTTON(gtk_button_new_with_label("SIGN UP"));
    form_template->send_message = GTK_BUTTON(gtk_button_new_with_label("SEND MESSAGE"));
    gtk_container_add(GTK_CONTAINER(form_template->window), form_template->body);
    gtk_container_add(GTK_CONTAINER(form_template->body), form_template->login_input);
    gtk_container_add(GTK_CONTAINER(form_template->body), form_template->password_input);
    gtk_container_add(GTK_CONTAINER(form_template->body), form_template->message);
    gtk_container_add(GTK_CONTAINER(form_template->body), form_template->user_id);
    gtk_container_add(GTK_CONTAINER(form_template->body), form_template->chat_id);
    gtk_container_add(GTK_CONTAINER(form_template->body), GTK_WIDGET(form_template->log_in_btn));
    gtk_container_add(GTK_CONTAINER(form_template->body), GTK_WIDGET(form_template->sign_up_btn));
    gtk_container_add(GTK_CONTAINER(form_template->body), GTK_WIDGET(form_template->send_message));
    gtk_entry_set_placeholder_text(GTK_ENTRY(form_template->login_input), "Email address");
    gtk_entry_set_placeholder_text(GTK_ENTRY(form_template->password_input), "Password");
    gtk_entry_set_placeholder_text(GTK_ENTRY(form_template->message), "message");
    gtk_entry_set_placeholder_text(GTK_ENTRY(form_template->user_id), "user_id");
    gtk_entry_set_placeholder_text(GTK_ENTRY(form_template->chat_id), "chat_id");
    g_signal_connect(form_template->log_in_btn, "clicked", G_CALLBACK(sign_in_req), form_template);
    g_signal_connect(form_template->sign_up_btn, "clicked", G_CALLBACK(sign_up_req), form_template);
    g_signal_connect(form_template->send_message, "clicked", G_CALLBACK(send_message_req), form_template);
    g_signal_connect(form_template->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(form_template->window);
    gtk_main();
    return 0;
}
