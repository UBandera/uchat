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


static void get_user_id(GtkWidget *widget, t_client *client) {
    GtkButton *header = GTK_BUTTON(client->contact_info);
    gchar *label = (gchar *)gtk_button_get_label(GTK_BUTTON(widget));
    GtkWidget *parent_row = gtk_widget_get_parent(widget);

    gtk_list_box_select_row(client->contacts, GTK_LIST_BOX_ROW(parent_row));
    gtk_widget_set_visible(client->chat_box, TRUE);
    gtk_button_set_label(header, label);
}


void prints() {
    printf("Hello\n");
}

void get_chat(GtkWidget *widget, GdkEventButton *event, t_contact_data *contact) {
    t_client *client = *mx_get_client();

    if (event->type == GDK_BUTTON_PRESS  &&  event->button == 1) {
        if (client->chat_with != contact->id) {
            GtkBuilder *builder = client->builder;
            GtkWidget *chat = client->chat_box;
            gchar *request = NULL;

            client->chat_with = contact->id;
            get_user_id(widget, client);
            clear_chat_box(GTK_LIST_BOX(client->chat));
            gtk_widget_set_visible(chat, TRUE);
            request = mx_chat_history_request(contact->id,
                                            client->token, 0, 10);
            mx_send_data(client->data_out, request);
            gtk_widget_show(chat);
            g_free(request);
            (void)widget;
        }
    }
    else if (event->type == GDK_BUTTON_PRESS  &&  event->button == 3) {
        GdkEvent *bevent = (GdkEvent *) event;
        gtk_menu_popup_at_pointer(GTK_MENU(contact->popup), bevent);
    }
}

static t_contact_data *setup_contact_struct(gchar *first_name,
                                            gchar *last_name, gint user_id) {
    t_contact_data *contact = g_new(t_contact_data, 1);

    contact->row = GTK_WIDGET(gtk_list_box_row_new());
    contact->first_name = g_strdup(first_name);
    contact->last_name = g_strdup(last_name);
    contact->id = user_id;
    return contact;
}

GtkWidget *crate_popup(t_contact_data *contact) {
    GtkWidget *pmenu = NULL;
    GtkWidget *ebox = NULL;
    GtkWidget *open_contact = NULL;
    GtkWidget *del_contact = NULL;
    GtkWidget *clear_chat = NULL;

    ebox = GTK_WIDGET(gtk_event_box_new());
    gtk_container_add(GTK_CONTAINER(contact->row), ebox);
    pmenu = GTK_WIDGET(gtk_menu_new());
    open_contact = gtk_menu_item_new_with_label("Open contact info");
    gtk_widget_show(open_contact);
    gtk_menu_shell_append(GTK_MENU_SHELL(pmenu), open_contact);
    g_signal_connect_swapped(G_OBJECT(open_contact), "activate",            //сигнал на опен контактс
        G_CALLBACK(prints), NULL);
    del_contact = gtk_menu_item_new_with_label("Delete contact");
    gtk_widget_show(del_contact);
    gtk_menu_shell_append(GTK_MENU_SHELL(pmenu), del_contact);
    g_signal_connect_swapped(G_OBJECT(del_contact), "activate",         //сигнал на удаление контакта
        G_CALLBACK(prints), NULL);
    clear_chat = gtk_menu_item_new_with_label("Clear chat");
    gtk_widget_show(clear_chat);
    gtk_menu_shell_append(GTK_MENU_SHELL(pmenu), clear_chat);
    g_signal_connect_swapped(G_OBJECT(clear_chat), "activate", 
        G_CALLBACK(prints), NULL);                                  //сигнал на очистить чат
    return pmenu;
}

void mx_show_contact_in_ui(t_client *client, gchar *first_name,
                           gchar *last_name, gint user_id) {
    if (!g_hash_table_contains(client->contacts_table,
                               GINT_TO_POINTER(user_id))) {
        t_contact_data *new_contact = NULL;
        gchar *label = g_strjoin(" ", last_name, first_name, NULL);
        GtkWidget *contact = gtk_button_new_with_label(label);

        new_contact = setup_contact_struct(first_name, last_name, user_id);
        new_contact->popup = crate_popup(new_contact->row);
        g_hash_table_insert(client->contacts_table,
                            GINT_TO_POINTER(user_id), new_contact);
        g_signal_connect(G_OBJECT(contact), "button-press-event",
                            G_CALLBACK(get_chat), new_contact);
        gtk_container_add(GTK_CONTAINER(new_contact->row), contact);
        gtk_container_add(GTK_CONTAINER(client->contacts), new_contact->row);
        gtk_widget_show_all(new_contact->row);
    }
}
