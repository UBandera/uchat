#include "client.h"

#define MX_STYLES "./src/screens/auth/test0.css"

/*
 * PURPOSE : Autogenerates function contract comments
 *  PARAMS : json - formed json receiving from server response string
 *           client - struct that contain all nessasery data (in this case
 *           GtkBuilder for showing reaction in ui).
 * RETURNS : void
 *   NOTES : -
 */
void mx_get_chat_history(cJSON *json, t_client *client) {
    cJSON *array = cJSON_GetObjectItemCaseSensitive(json, "messages");
    cJSON *data = NULL;
    GtkListBoxRow *row = gtk_list_box_get_selected_row(client->contacts);

    GtkWidget *child = GTK_WIDGET(gtk_bin_get_child(GTK_BIN(row)));
    mx_apply_styles(MX_STYLES);
    gtk_widget_set_name(child, "notify_disable");

    cJSON_ArrayForEach(data, array) {
        gchar *message = cJSON_GetObjectItem(data, "message")->valuestring;
        gint date = cJSON_GetObjectItem(data, "date")->valueint;

        g_print("chat history handler\n");
        mx_show_message_in_ui(client, message);
    }
}
