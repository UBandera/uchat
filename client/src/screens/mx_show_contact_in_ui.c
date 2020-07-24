#include "client.h"

static void get_chat(GtkWidget *widget, gpointer user_id) {
    t_client *client = *mx_get_client();
    // gchar *request = NULL;

    // request = mx_chat_history_request(user_id);
    g_print("open_chat with user_id = %d\n", GPOINTER_TO_INT(user_id));
    // mx_send_data(client->data_out, request);
    (void)widget;
}

void mx_show_contact_in_ui(t_client *client, gchar *label, gint user_id) {
    GtkListBox *box = GTK_LIST_BOX(gtk_builder_get_object(client->builder,
                                                          "contacts_box"));
    GtkWidget *contact = gtk_button_new_with_label(label);
    GtkWidget *row = GTK_WIDGET(gtk_list_box_row_new());

    g_signal_connect(contact, "clicked", G_CALLBACK(get_chat),
                     GINT_TO_POINTER(user_id));
     gtk_list_box_insert(box, row, -1);
    gtk_container_add(GTK_CONTAINER(row), contact);
    gtk_container_add(GTK_CONTAINER(box), row);
    gtk_widget_show_all(GTK_WIDGET(box));
}
