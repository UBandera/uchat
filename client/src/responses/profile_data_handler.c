#include "client.h"

void mx_profile_data(cJSON *json, t_client *client) {
    const gchar *phone = cJSON_GetObjectItem(json, "phone")->valuestring;
    const gchar *first_name = cJSON_GetObjectItem(json, "first_name")->valuestring;
    const gchar *last_name = cJSON_GetObjectItem(json, "last_name")->valuestring;
    const gchar *email = cJSON_GetObjectItem(json, "email")->valuestring;

    mx_set_entry_value(client->builder, "profile_phone", phone);
    mx_set_entry_value(client->builder, "profile_first_name", first_name);
    mx_set_entry_value(client->builder, "profile_last_name", last_name);
    mx_set_entry_value(client->builder, "profile_email", email);
    gtk_widget_show(GTK_WIDGET(client->profile_window));
}
