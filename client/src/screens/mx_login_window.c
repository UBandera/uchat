#include "client.h"

#define MX_SIGN_IN_WINDOW "sign_in.glade"

static void get_data_from_input(GtkButton *button, t_client *client) {
    GtkEntry *login_entry = GTK_ENTRY(gtk_builder_get_object(client->builder, "login_entry"));
    GtkEntry *password_enty = GTK_ENTRY(gtk_builder_get_object(client->builder, "password_entry"));
    gchar *login = (gchar *)gtk_entry_get_text(login_entry);
    gchar *password = (gchar *)gtk_entry_get_text(password_enty);

    if (mx_auth_confirming(login, password, NULL) == MX_VALID) {
        gchar *request = mx_form_auth_request(login, password, RQ_SIGN_IN);
        GtkLabel *label = GTK_LABEL(gtk_builder_get_object(client->builder, "info_label"));

        mx_send_data(client->data_out, request);
        gtk_widget_hide(GTK_WIDGET(label));
        g_free(request);
    }
    (void)button;
}

void mx_login_window(t_client *client) {
    GError *error = NULL;
    GtkWindow *window = NULL;
    GtkButton *login_btn = NULL;

    if (!gtk_builder_add_from_file(client->builder,
                                   MX_SIGN_IN_WINDOW,
                                   &error)) {
        g_error("%s\n", error->message);
   }
   window = GTK_WINDOW(gtk_builder_get_object(client->builder, "window"));
   login_btn = GTK_BUTTON(gtk_builder_get_object(client->builder, "login_btn"));

   g_signal_connect(login_btn, "clicked", G_CALLBACK(get_data_from_input), NULL);
   g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
}
