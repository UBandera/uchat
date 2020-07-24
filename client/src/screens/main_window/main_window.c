#include "client.h"

#define MX_MAIN_WINDOW "./src/screens/glade/main.glade"

static void add_contact_btn(GtkButton *button, t_client *client) {
    GtkWindow *current = gtk_application_get_active_window(client->app);

    gtk_window_set_accept_focus(current, FALSE);
    gtk_widget_show(GTK_WIDGET(client->add_contact_dialog));
    (void)client;
}

static void controling(GtkBuilder *builder, t_client *client) {
    // GtkWidget *chat = GTK_WIDGET(gtk_builder_get_object(builder, "chat"));
    GtkButton *add_contact = GTK_BUTTON(gtk_builder_get_object(builder, "add_contact_dialog"));
    GtkListBox *box = GTK_LIST_BOX(gtk_builder_get_object(client->builder, "contacts_box"));

    g_signal_connect(add_contact, "clicked", G_CALLBACK(add_contact_btn), client);

    // mx_chat_contol(builder, client);
    // GtkWidget *back = NULL;
    // GtkWidget *phone_edit = NULL;

    // next = GTK_BUTTON(gtk_builder_get_object(builder, "second_next_button"));
    // back = GTK_BUTTON(gtk_builder_get_object(builder, "second_back_button"));
    // phone_edit = GTK_ENTRY(gtk_builder_get_object(builder, "phone_edit"));
    // g_signal_connect(phone_edit, "icon-press",
    //                  G_CALLBACK(mx_edit_login), client);
    // g_signal_connect(next, "clicked", G_CALLBACK(verify_user), client);
    // g_signal_connect(back, "clicked", G_CALLBACK(go_back), client);
}



GtkWindow *mx_main_window(t_client *client) {
    GtkBuilder *builder = client->builder;
    GError *error = NULL;
    GtkWindow *window = NULL;

    // mx_apply_styles(MX_STYLES);
    if (!gtk_builder_add_from_file(builder,
                                   MX_MAIN_WINDOW,
                                   &error))
        g_error("%s\n", error->message);
    window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));

    controling(builder, client);


    // sendButton = GTK_WIDGET(gtk_builder_get_object(builder,"sendButton"));
    // ContactButton = GTK_WIDGET(gtk_builder_get_object(builder,"btn_add_room"));
    // g_signal_connect(G_OBJECT(ContactButton),"clicked", G_CALLBACK(clear_list_entry) ,NULL);
    // sendEntry = GTK_WIDGET(gtk_builder_get_object(builder,"sendEntry"));
    // Contacts = GTK_WIDGET(gtk_builder_get_object(builder,"listbox_rooms"));
    // g_signal_connect(G_OBJECT(sendEntry),"activate", G_CALLBACK(send_messege) ,NULL);
    // g_signal_connect(G_OBJECT(sendButton),"clicked", G_CALLBACK(send_messege) ,NULL);
    // TextView = GTK_WIDGET(gtk_builder_get_object(builder,"msg_entry"));
    // messagesTreeView = GTK_WIDGET(gtk_builder_get_object(builder,"messagesTreeView"));
    // messagesListStore = GTK_LIST_STORE(gtk_builder_get_object(builder,"messagesListStore"));
    // scrolledWindow = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder,"scrolledWindow"));
    // vAdjust = gtk_scrolled_window_get_vadjustment(scrolledWindow);

    return window;
}
