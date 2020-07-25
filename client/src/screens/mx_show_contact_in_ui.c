#include "client.h"

static void get_user_id(GtkWidget *widget, t_client *client) {
    GtkButton *header = GTK_BUTTON(client->contact_info);
    gchar *label = (gchar *)gtk_button_get_label(GTK_BUTTON(widget));
    GtkWidget *parent_row = gtk_widget_get_parent(widget);

    gtk_list_box_select_row(client->contacts, GTK_LIST_BOX_ROW(parent_row));
    g_print("get row index = %d\n" , gtk_list_box_row_get_index(GTK_LIST_BOX_ROW(parent_row)));

    gtk_widget_set_visible(client->chat_box, TRUE);
    gtk_button_set_label(header, label);
}

static void get_chat(GtkWidget *widget, gpointer user_id) {
    t_client *client = *mx_get_client();
    GtkBuilder *builder = client->builder;
    GtkWidget *chat = client->chat_box;
    // gchar *request = NULL;

    get_user_id(widget, client);
    // request = mx_chat_history_request(GPOINTER_TO_INT(user_id));
    g_print("open_chat with user_id = %d\n", GPOINTER_TO_INT(user_id));
    gtk_widget_show(chat);
    // mx_send_data(client->data_out, request);
    // g_free(request);
    (void)widget;
}

static t_contact_data *setup_contact_struct(gchar *first_name,
                                            gchar *last_name) {
    t_contact_data *contact = g_new(t_contact_data, 1);

    contact->row = GTK_WIDGET(gtk_list_box_row_new());
    contact->first_name = first_name;
    contact->last_name = last_name;
    return contact;
}

void mx_show_contact_in_ui(t_client *client, gchar *first_name,
                           gchar *last_name, gint user_id) {
    if (!g_hash_table_contains(client->contacts_table,
                               GINT_TO_POINTER(user_id))) {
        t_contact_data *new_contact = NULL;
        gchar *label = g_strjoin(" ", last_name, first_name, NULL);
        GtkWidget *contact = gtk_button_new_with_label(label);

        new_contact = setup_contact_struct(first_name, last_name);
        g_hash_table_insert(client->contacts_table,
                            GINT_TO_POINTER(user_id), new_contact);
        g_signal_connect(contact, "clicked", G_CALLBACK(get_chat),
                         GINT_TO_POINTER(user_id));
        gtk_container_add(GTK_CONTAINER(new_contact->row), contact);
        gtk_container_add(GTK_CONTAINER(client->contacts), new_contact->row);
        gtk_widget_show_all(new_contact->row);
    }
}
