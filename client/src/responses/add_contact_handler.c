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
    gint contact = cJSON_GetObjectItem(json, "contact")->valueint;
    gchar *label = (gchar *)gtk_button_get_label(GTK_BUTTON(client->contact_view));

    mx_show_contact_in_ui(client, NULL, label, contact);
    g_print("message = %s\n", message);
    mx_window_switcher(client->add_contact_dialog, client->main_window);
    gtk_widget_grab_focus(GTK_WIDGET(client->main_window));
}
