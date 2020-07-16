#include "client.h"

#define MX_PHONE_INPUT_WINDOW "./src/screens/glade/phone_entering_window.glade"
#define MX_STYLES "./src/screens/auth/auth.css"

static void generate_pass(GtkButton *button, t_client *client) {
    GtkEntryBuffer *buffer = NULL;
    GtkBuilder *builder = client->builder;
    GtkEntry *phone = GTK_ENTRY(gtk_builder_get_object(builder, "phone"));
    const gchar *phone_value = gtk_entry_get_text(phone);
    gchar *request = mx_password_request(phone_value);

    buffer = GTK_ENTRY_BUFFER(gtk_builder_get_object(builder, "phone_buff"));
    mx_send_data(client->data_out, request);
    gtk_entry_buffer_set_text(buffer, phone_value, -1);
    gtk_entry_set_text(phone, "");
    g_free(request);
    (void)button;
}

static void controling(GtkBuilder *builder, t_client *client) {
    GtkButton *next = NULL;
    GtkEntry *phone = NULL;
    GtkEntryBuffer *buffer = NULL;

    buffer = GTK_ENTRY_BUFFER(gtk_builder_get_object(builder, "phone_buff"));
    next = GTK_BUTTON(gtk_builder_get_object(builder, "first_next_button"));
    phone = GTK_ENTRY(gtk_builder_get_object(builder, "phone"));
    gtk_entry_set_text(phone, gtk_entry_buffer_get_text(buffer));
    g_signal_connect(next, "clicked", G_CALLBACK(generate_pass), client);
}

GtkWindow *mx_phone_entering_window(t_client *client) {
    GtkBuilder *builder = client->builder;
    GError *error = NULL;
    GtkWindow *window = NULL;

    mx_apply_styles(MX_STYLES);
    if (!gtk_builder_add_from_file(builder,
                                   MX_PHONE_INPUT_WINDOW,
                                   &error))
        g_error("%s\n", error->message);
    window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));
    controling(builder, client);
    return window;
}
