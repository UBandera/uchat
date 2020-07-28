#include "client.h"

/*
 * PURPOSE : Autogenerates function contract comments
 *  PARAMS : json - formed json receiving from server response string
 *           client - struct that contain all nessasery data (in this case
 *           GtkBuilder for showing reaction in ui).
 * RETURNS : void
 *   NOTES : -
 */
void mx_add_contact(cJSON *json, t_client *client) {
    gchar *message = cJSON_GetObjectItem(json, "message")->valuestring;
    gint contact = 0;
    gchar *first_name = NULL;
    gchar *last_name = NULL;
    gchar *label = NULL;

    contact = cJSON_GetObjectItem(json, "user_id")->valueint;
    first_name = cJSON_GetObjectItem(json, "first_name")->valuestring;
    last_name = cJSON_GetObjectItem(json, "last_name")->valuestring;
    label = g_strjoin(" ", last_name, first_name, NULL);
    mx_show_contact_in_ui(client, NULL, label, contact);
    g_print("message = %s\n", message);
    mx_window_switcher(client->add_contact_dialog, client->main_window);
    gtk_widget_grab_focus(GTK_WIDGET(client->main_window));
    g_free(label);
}
