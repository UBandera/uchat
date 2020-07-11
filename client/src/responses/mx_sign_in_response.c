#include "client.h"

#define MX_SIGN_IN_ERROR 1
#define MX_SIGN_IN_SUCCESSFULLY 0

/*
 * PURPOSE : Autogenerates function contract comments
 *  PARAMS : json - formed json receiving from server response string
 *           client - struct that contain all nessasery data (in this case
 *           GtkBuilder for showing reaction in ui).
 * RETURNS : void
 *   NOTES : -
 */
void mx_sign_in_response(cJSON *json, t_client *client) {
    gint status = cJSON_GetObjectItem(json, "status")->valueint;
    gchar *message = cJSON_GetObjectItem(json, "message")->valuestring;

    if (status == MX_SIGN_IN_ERROR) {
        // print label in ui with showing this error_message
        GtkLabel *label = GTK_LABEL(gtk_builder_get_object(client->builder, "info_label"));

        gtk_label_set_text(label, message);
        gtk_widget_show(GTK_WIDGET(label));
    }
    else if (status == MX_SIGN_IN_SUCCESSFULLY) {
        // redirect to main_window
        g_print("User will successfully sign in.\n");
        // mx_main_window(client);
    }
    // if unknown error eccured
    else {
        // crush or printing error deppends on situation.
        g_error("Unknown error occured!");
    }
}
