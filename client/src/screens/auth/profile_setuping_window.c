#include "client.h"

#define MX_PROFILE_SETUP_WINDOW "./src/screens/glade/profile_setup.glade"
#define MX_STYLES "./src/screens/auth/auth.css"

static void add_info(GtkButton *button, t_client *client) {
    gchar *request = NULL;
    GtkEntry *phone = GTK_ENTRY(gtk_builder_get_object(client->builder,
                                                       "phone_not_edit"));
    GtkEntry *first_name = GTK_ENTRY(gtk_builder_get_object(client->builder,
                                                            "first_name"));
    GtkEntry *last_name = GTK_ENTRY(gtk_builder_get_object(client->builder,
                                                           "last_name"));
    GtkEntry *email = GTK_ENTRY(gtk_builder_get_object(client->builder,
                                                       "email"));

    request = mx_set_up_profile_request(gtk_entry_get_text(phone),
                                        gtk_entry_get_text(first_name),
                                        gtk_entry_get_text(last_name),
                                        gtk_entry_get_text(email));
    mx_send_data(client->data_out, request);
    mx_window_switcher(client->profile_setuping, client->phone_entering);
    g_free(request);
    (void)button;
}

static void go_back(GtkButton *button, t_client *client) {
    GtkBuilder *builder = client->builder;
    GtkEntry *first_name = NULL;
    GtkEntry *last_name = NULL;
    GtkEntry *email = NULL;

    first_name = GTK_ENTRY(gtk_builder_get_object(builder, "first_name"));
    last_name = GTK_ENTRY(gtk_builder_get_object(builder, "last_name"));
    email = GTK_ENTRY(gtk_builder_get_object(builder, "email"));
    gtk_entry_set_text(first_name, "");
    gtk_entry_set_text(last_name, "");
    gtk_entry_set_text(email, "");
    mx_window_switcher(client->profile_setuping, client->phone_entering);
    (void)button;
}

static void controling(GtkBuilder *builder, t_client *client) {
    GtkButton *next = NULL;
    GtkButton *back = NULL;
    GtkButton *photo = NULL;

    back = GTK_BUTTON(gtk_builder_get_object(builder, "third_back_button"));
    next = GTK_BUTTON(gtk_builder_get_object(builder, "sign"));
    photo = GTK_BUTTON(gtk_builder_get_object(builder, "photo"));

    g_signal_connect(next, "clicked", G_CALLBACK(add_info), client);
    g_signal_connect(back, "clicked", G_CALLBACK(go_back), client);
}

GtkWindow *mx_profile_setuping_window(t_client *client) {
    GtkBuilder *builder = client->builder;
    GError *error = NULL;
    GtkWindow *window = NULL;

    mx_apply_styles(MX_STYLES);
    if (!gtk_builder_add_from_file(builder, MX_PROFILE_SETUP_WINDOW, &error))
        g_error("%s\n", error->message);
    window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));
    controling(builder, client);
    return window;
}
