#include "client.h"

static void add_contact(GtkWidget *widget, gpointer user_id) {
    t_client *client = *mx_get_client();
    gchar *request = NULL;
    gint new_contact = GPOINTER_TO_INT(user_id);

    request = mx_add_contact_to_friend_request(new_contact, "client->token");
    mx_send_data(client->data_out, request);
    gtk_widget_hide(widget);
    mx_window_switcher(client->add_contact_dialog, client->main_window);
    gtk_window_set_accept_focus(client->main_window, TRUE);
    g_free(request);
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
    // gchar *message = cJSON_GetObjectItem(json, "message")->valuestring;
    // GtkWindow *current = gtk_application_get_active_window(client->app);
    gint user_id = cJSON_GetObjectItem(json, "user_id")->valueint;
    gchar *name = cJSON_GetObjectItem(json, "name")->valuestring;
    gchar *last_name = cJSON_GetObjectItem(json, "last_name")->valuestring;
    GtkButton *contact = GTK_BUTTON(gtk_builder_get_object(client->builder, "add_contact_btn"));
    gchar *label = g_strjoin(" ", last_name, name, NULL);

    gtk_button_set_label(contact, label);
    g_free(label);
    g_signal_connect(contact, "clicked", G_CALLBACK(add_contact), GINT_TO_POINTER(user_id));
    gtk_widget_show(GTK_WIDGET(contact));
    // g_print("message = %s\n", message);
    // mx_window_switcher(current, client->main_window);
}
