#include "client.h"

void mx_edit_login(GtkEntry *entry, GtkEntryIconPosition icon_pos,
                   GdkEvent *event, t_client *client) {
    GtkBuilder *builder = client->builder;
    GtkEntry *password = NULL;

    password = GTK_ENTRY(gtk_builder_get_object(builder, "password"));
    gtk_entry_set_text(password, "");
    mx_window_switcher(client->password_validation, client->phone_entering);
    (void)entry;
    (void)icon_pos;
    (void)event;
}
