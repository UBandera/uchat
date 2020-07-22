#include "client.h"

#define MX_CONTACTS_NOT_FOUND 1
#define MX_SUCCESS 0

static void show_chat(GtkWidget *widget, gchar *first_name) {
    g_print("Chat with username=%s\n", first_name);
    (void)widget;
}

void mx_show_contact_in_ui(GtkBuilder *builder, gchar *first_name,
                           gchar *last_name, gint *unser_id) {
    GtkListBox *box = GTK_LIST_BOX(gtk_builder_get_object(builder, "listbox_rooms"));
    GtkWidget *new_client = gtk_button_new_with_label(first_name);

    gtk_list_box_insert(box, new_client, -1);

    g_signal_connect(new_client, "clicked", G_CALLBACK(show_chat), first_name);
    gtk_widget_show(new_client);
    (void)client;
}

/*
 * PURPOSE : Autogenerates function contract comments
 *  PARAMS : json - formed json receiving from server response string
 *           client - struct that contain all nessasery data (in this case
 *           GtkBuilder for showing reaction in ui in `mx_show_contact_in_ui`
 *           function).
 * RETURNS : void
 *   NOTES : -
 */
static void get_contact_from_json(cJSON *json, t_client *client) {
    cJSON *contacts = cJSON_GetObjectItemCaseSensitive(json, "contacts");
    cJSON *data = NULL;

    cJSON_ArrayForEach(data, contacts) {
        gchar *first_name = cJSON_GetObjectItem(data, "first_name")->valuestring;
        gchar *last_name = cJSON_GetObjectItem(data, "last_name")->valuestring;
        gint user_id = cJSON_GetObjectItem(data, "user_id")->valueint;

        mx_show_contact_in_ui(client->builder, first_name, last_name, user_id);
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
void mx_contact_list_response(cJSON *json, t_client *client) {
    gchar *message = cJSON_GetObjectItem(json, "message")->valuestring;

    get_contact_from_json(json, client);
}
