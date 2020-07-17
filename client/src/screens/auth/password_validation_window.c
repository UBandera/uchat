#include "client.h"


#define MX_PASSWORD_VALIDATION_WINDOW "./src/screens/glade/password_validation.glade"
#define MX_STYLES "./src/screens/auth/auth.css"

static void verify_user(GtkButton *button, t_client *client) {
    GtkBuilder *builder = client->builder;
    gchar *request = NULL;
    GtkEntry *phone = NULL;
    GtkEntry *password = NULL;
    GtkEntryBuffer *buffer = NULL;

    phone = GTK_ENTRY(gtk_builder_get_object(builder, "phone_edit"));
    password = GTK_ENTRY(gtk_builder_get_object(builder, "password"));
    request = mx_auth_request(gtk_entry_get_text(phone),
                              gtk_entry_get_text(password));
    mx_send_data(client->data_out, request);
    buffer = GTK_ENTRY_BUFFER(gtk_builder_get_object(builder, "phone_buff"));
    // gtk_entry_buffer_set_text(buffer, "\0", -1);
    // gtk_entry_set_text(phone, "");
    gtk_entry_set_text(password, "");
    g_free(request);
    (void)button;
}

static void go_back(GtkButton *button, t_client *client) {
    GtkBuilder *builder = client->builder;
    GtkEntry *phone = NULL;
    GtkEntry *password = NULL;
    GtkEntryBuffer *buffer = NULL;

    phone = GTK_ENTRY(gtk_builder_get_object(builder, "phone_edit"));
    password = GTK_ENTRY(gtk_builder_get_object(builder, "password"));
    buffer = GTK_ENTRY_BUFFER(gtk_builder_get_object(builder, "phone_buff"));
    gtk_entry_buffer_set_text(buffer, "\0", -1);
    gtk_entry_set_text(phone, "");
    gtk_entry_set_text(password, "");
    mx_window_switcher(client->password_validation, client->phone_entering);
    (void)button;
}

static void controling(GtkBuilder *builder, t_client *client) {
    GtkButton *next = NULL;
    GtkButton *back = NULL;
    GtkEntry *phone_edit = NULL;

    next = GTK_BUTTON(gtk_builder_get_object(builder, "second_next_button"));
    back = GTK_BUTTON(gtk_builder_get_object(builder, "second_back_button"));
    phone_edit = GTK_ENTRY(gtk_builder_get_object(builder, "phone_edit"));
    g_signal_connect(phone_edit, "icon-press",
                     G_CALLBACK(mx_edit_login), client);
    g_signal_connect(next, "clicked", G_CALLBACK(verify_user), client);
    g_signal_connect(back, "clicked", G_CALLBACK(go_back), client);
}

GtkWindow *mx_password_validate_window(t_client *client) {
    GtkBuilder *builder = client->builder;
    GError *error = NULL;
    GtkWindow *window = NULL;

    mx_apply_styles(MX_STYLES);
    if (!gtk_builder_add_from_file(builder,
                                   MX_PASSWORD_VALIDATION_WINDOW,
                                   &error))
        g_error("%s\n", error->message);
    window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));
    controling(builder, client);
    return window;
}
