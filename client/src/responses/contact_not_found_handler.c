#include "client.h"

/*
 * PURPOSE : Autogenerates function contract comments
 *  PARAMS : json - formed json receiving from server response string
 *           client - struct that contain all nessasery data (in this case
 *           GtkBuilder for showing reaction in ui).
 * RETURNS : void
 *   NOTES : -
 */
void mx_contact_not_found(cJSON *json, t_client *client) {
    GtkBuilder *builder = client->builder;
    gchar *message = cJSON_GetObjectItem(json, "message")->valuestring;
    GtkLabel *info = NULL;

    info = GTK_LABEL(gtk_builder_get_object(builder, "info_label"));
    gtk_label_set_text(info, message);
    gtk_widget_show(GTK_WIDGET(info));
}
