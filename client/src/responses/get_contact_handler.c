#include "client.h"

void clear_entry(GtkBuilder *builder, const gchar *entry_name) {
    GtkEntry *entry = NULL;
    GtkEntryBuffer* buf = NULL;

    entry = GTK_ENTRY(gtk_builder_get_object(builder, entry_name));
    buf = gtk_entry_get_buffer(entry);
    gtk_entry_buffer_set_text(buf, "\0", -1 );
}

static void open_exist_chat(t_client *client, gint user_id) {
    GHashTable *contacts = client->contacts_table;
    t_contact_data *node = NULL;
    GtkButton *header = GTK_BUTTON(client->contact_info);
    gchar *label = NULL;

    node = (t_contact_data *)g_hash_table_lookup(contacts,
                                                 GINT_TO_POINTER(user_id));
    label = g_strjoin(" ", node->last_name, node->first_name, NULL);
    gtk_list_box_select_row(client->contacts, GTK_LIST_BOX_ROW(node->row));
    gtk_widget_set_visible(client->chat_box, TRUE);
    gtk_button_set_label(header, label);
    g_free(label);
}

static void add_contact(GtkWidget *widget, gpointer user_id) {
    t_client *client = *mx_get_client();
    gchar *request = NULL;
    gint new_contact = GPOINTER_TO_INT(user_id);

    gtk_widget_hide(widget);
    clear_entry(client->builder, "dialog_search");
    if (!g_hash_table_contains(client->contacts_table, user_id)) {
        request = mx_add_contact_request(new_contact, client->token);
        mx_send_data(client->data_out, request);
        g_free(request);
    }
    else
        open_exist_chat(client, new_contact);
    g_print("open_chat with user_id = %d\n", GPOINTER_TO_INT(user_id));
    mx_window_switcher(client->add_contact_dialog, client->main_window);
    gtk_widget_grab_focus(GTK_WIDGET(client->main_window));
}

/*
 * PURPOSE : Autogenerates function contract comments
 *  PARAMS : json - formed json receiving from server response string
 *           client - struct that contain all nessasery data (in this case
 *           GtkBuilder for showing reaction in ui).
 * RETURNS : void
 *   NOTES : -
 */
void mx_get_contact(cJSON *json, t_client *client) {
    gint user_id = cJSON_GetObjectItem(json, "user_id")->valueint;
    gchar *name = cJSON_GetObjectItem(json, "name")->valuestring;
    gchar *last_name = cJSON_GetObjectItem(json, "last_name")->valuestring;
    gchar *label = g_strjoin(" ", last_name, name, NULL);
    GtkBox *box = NULL;

    client->contact_view = GTK_WIDGET(gtk_button_new_with_label(label));
    box = GTK_BOX(gtk_builder_get_object(client->builder, "add_dialog_box"));
    g_free(label);
    gtk_container_add(GTK_CONTAINER(box), client->contact_view);
    g_signal_connect(client->contact_view, "clicked", G_CALLBACK(add_contact),
                     GINT_TO_POINTER(user_id));
    gtk_widget_show(client->contact_view);
}
