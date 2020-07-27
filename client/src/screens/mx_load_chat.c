#include "client.h"

static void clear_chat_box(GtkListBox *box) {
    GtkWidget *row = NULL;

    for (int i = 0; ; i++) {
        if ((row = GTK_WIDGET(gtk_list_box_get_row_at_index(box, i)))) {
            GtkWidget *child = GTK_WIDGET(gtk_bin_get_child(GTK_BIN(row)));

            gtk_widget_hide(row);
            gtk_container_remove(GTK_CONTAINER(row), child);
            gtk_container_remove(GTK_CONTAINER(box), row);
        }
        else {
            break;
        }
    }
}

void mx_load_chat(t_client *client, gpointer user_id) {
    if (client->chat_with != GPOINTER_TO_INT(user_id)) {
        GHashTable *contacts = client->contacts_table;
        GtkButton *header = GTK_BUTTON(client->contact_info);
        t_contact_data *node = NULL;
        gchar *label = NULL;
        gchar *request = NULL;

        clear_chat_box(GTK_LIST_BOX(client->chat));
        client->chat_with = GPOINTER_TO_INT(user_id);
        node = (t_contact_data *)g_hash_table_lookup(contacts, user_id);
        label = g_strjoin(" ", node->last_name, node->first_name, NULL);
        gtk_list_box_select_row(client->contacts, GTK_LIST_BOX_ROW(node->row));
        gtk_widget_set_visible(client->chat_box, TRUE);
        gtk_button_set_label(header, label);
        request = mx_chat_history_request(GPOINTER_TO_INT(user_id),
                                          client->token, 0, 10);
        mx_send_data(client->data_out, request);
        g_free(request);
    }
}
