#include "client.h"

void mx_remove_rows(GtkListBox *listbox) {
    GList *children = NULL;
    GList *iter = NULL;

    children = gtk_container_get_children(GTK_CONTAINER(listbox));
    for (iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    g_list_free(children);
}

void mx_free_client_data(t_client *client) {
    mx_remove_rows(client->contacts);
    mx_remove_rows(client->chat);
    gtk_widget_hide(client->chat_box);
    if (client->contacts_table) {
        g_hash_table_destroy(client->contacts_table);
        client->contacts_table = g_hash_table_new(NULL, NULL);
    }
}

/*
 * PURPOSE : Autogenerates function contract comments
 *  PARAMS : json - formed json receiving from server response string
 *           client - struct that contain all nessasery data (in this case
 *           GtkBuilder for showing reaction in ui).
 * RETURNS : void
 *   NOTES : -
 */
void mx_sign_out_handler(cJSON *json, t_client *client) {
    // GtkBuilder *builder = client->builder;
    // gchar *message = cJSON_GetObjectItem(json, "message")->valuestring;

    mx_free_client_data(client);
    // gtk_widget_hide(GTK_WIDGET(client->main_window));
    mx_window_switcher(client->main_window, client->phone_entering);
}
