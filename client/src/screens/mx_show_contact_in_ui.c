#include "client.h"

static void view_chat(GtkWidget *widget, t_client *client) {
    GtkButton *header = GTK_BUTTON(client->contact_info);
    gchar *label = (gchar *)gtk_button_get_label(GTK_BUTTON(widget));

    gtk_button_set_label(header, label);
}

static void get_chat(GtkWidget *widget, gpointer user_id) {
    t_client *client = *mx_get_client();
    GtkBuilder *builder = client->builder;
    GtkWidget *chat = client->chat_box;
    // gchar *request = NULL;

    // request = mx_chat_history_request(GPOINTER_TO_INT(user_id));
    g_print("open_chat with user_id = %d\n", GPOINTER_TO_INT(user_id));
    gtk_widget_set_visible(chat, TRUE);
    view_chat(widget, client);
    gtk_widget_show(chat);
    // mx_send_data(client->data_out, request);
    // g_free(request);
    (void)widget;
}

void mx_show_contact_in_ui(t_client *client, gchar *label,
                           gint user_id, GtkListBox *box) {
    GtkWidget *contact = gtk_button_new_with_label(label);
    GtkWidget *row = GTK_WIDGET(gtk_list_box_row_new());
    // GtkWidget *chat = client->chat_box;

    gtk_list_box_insert(box, row, -1);
    g_signal_connect(contact, "clicked", G_CALLBACK(get_chat),
                     GINT_TO_POINTER(user_id));
    gtk_container_add(GTK_CONTAINER(row), contact);
    // gtk_container_add(GTK_CONTAINER(box), row);
    gtk_widget_show_all(contact);
    gtk_widget_show_all(row);
    gtk_widget_show_all(GTK_WIDGET(box));
}
