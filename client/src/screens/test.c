#include <gtk/gtk.h>
#include <gio/gio.h>
#include <string.h>
#define MX_TITLE "uChat"
#define MX_WELCOME "Welcome to the macOS application"

typedef struct s_client {
    GSocketConnection *connection;
    GInputStream *istream;
    GOutputStream *ostream;
    GDataInputStream *data_in;
    GDataOutputStream *data_out;
}              t_client;

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
    GtkEntryBuffer *entry_buffer;
    GtkCssProvider *provider;
}               t_form;

gssize mx_send_data(GDataOutputStream *data_out, gchar *data);

void get_auth(t_form *data, gchar *login) {
    t_client *client = data->client;
    // gchar *login = g_strdup(gtk_entry_get_text(GTK_ENTRY(data->login_input)));
    // gchar *login = g_strdup(gtk_entry_buffer_get_text(data->entry_buffer));
    GtkTextIter start;
    GtkTextIter end;

    // if (GTK_IS_TEXT_BUFFER(data->entry_buffer)) {
        // gtk_text_buffer_get_start_iter(GTK_TEXT_BUFFER(data->entry_buffer), &start);
        // gtk_text_buffer_get_end_iter(GTK_TEXT_BUFFER(data->entry_buffer), &end);
        // login = g_strdup(gtk_text_buffer_get_text(GTK_TEXT_BUFFER(data->entry_buffer), &start, &end, FALSE));
    // }
    // g_free(login);
}

void pushing(GtkButton *button, t_form *data) {
    // gchar *login = (gchar *)gtk_entry_get_text(GTK_ENTRY(((t_form *)data)->login_input));
    t_client *client = data->client;
    mx_send_data(client->data_out, "Hello server\n");
    return;
}

int login(int argc, char **argv, gpointer user_data) {
    t_form *form_template = (t_form *)malloc(sizeof(t_form));

    gtk_init(&argc, &argv);
    form_template->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    form_template->body = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
    form_template->entry_buffer = gtk_entry_buffer_new(NULL, -1);
    form_template->login_input = gtk_entry_new_with_buffer(form_template->entry_buffer);
    form_template->log_in_btn = GTK_BUTTON(gtk_button_new_with_label("Push"));
    form_template->client = (t_client *)user_data;
    mx_send_data(form_template->client->data_out, "Client inited\n");

    gtk_container_add(GTK_CONTAINER(form_template->window), form_template->body);

    gtk_container_add(GTK_CONTAINER(form_template->body), form_template->login_input);
    gtk_container_add(GTK_CONTAINER(form_template->body), GTK_WIDGET(form_template->log_in_btn));

    g_signal_connect(form_template->log_in_btn, "clicked", G_CALLBACK(pushing), form_template);

    g_signal_connect(form_template->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(form_template->window);
    gtk_main();
    return 0;
}
