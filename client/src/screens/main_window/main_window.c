#include "client.h"

#define MX_MAIN_WINDOW "./src/screens/glade/main_window.glade"

    GtkWidget *chatWindow;
    GtkWidget *sendButton;
    GtkWidget *sendEntry;
    GtkWidget *ContactButton;
    // GtkWidget *messagesTreeView;
    // GtkWidget *TextView;
    GtkWidget *Contacts;
    GtkAdjustment *vAdjust;
    GtkListStore *messagesListStore;


void sleep_ms(int milliseconds)
{
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
}

void add_list_entry(GtkBuilder *builder, const char *t, const char *a, const char *m, int sleep) // заносит сообщения в окно
{
    GtkTreeIter iter;
    GtkListStore *messagesTreeView = GTK_LIST_STORE(gtk_builder_get_object(builder,"messagesListStore"));
    GtkScrolledWindow *scrolledWindow = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder,"scrolledWindow"));
    GtkAdjustment *vAdjust = gtk_scrolled_window_get_vadjustment(scrolledWindow);
    GtkListStore *messagesListStore = GTK_LIST_STORE(gtk_builder_get_object(builder,"messagesListStore"));

    gtk_list_store_append(GTK_LIST_STORE(messagesListStore), &iter);
    gtk_list_store_set(GTK_LIST_STORE(messagesListStore), &iter, 0, t, 1, a, 2, m, -1);
    if(sleep)
        sleep_ms(100);
    gtk_adjustment_set_value(vAdjust, gtk_adjustment_get_upper(vAdjust) - gtk_adjustment_get_page_size(vAdjust));
}

void clear_list_entry(GtkBuilder *builder) {
    GtkListStore *messagesListStore = GTK_LIST_STORE(
                                    gtk_builder_get_object(builder,
                                                           "messagesListStore"));

    gtk_list_store_clear(messagesListStore);
}


static gchar *get_message(GtkTextView *TextView) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(TextView);
    gchar *message = NULL;
    gchar *temp = NULL;
    GtkTextIter start;
    GtkTextIter end;

    gtk_text_buffer_get_bounds(buffer, &start, &end);
    temp = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
    if (strlen(temp) > 0) {
        g_print("temp is %s\n", temp);
        message = g_strdup(temp);
    }
    gtk_text_buffer_delete(buffer, &start, &end);
    return message;
}

static void send_message(GtkButton *button, t_client *client) {
    GtkBuilder *builder = client->builder;
    GtkTextView *TextView = NULL;
    gchar *message = NULL;
    gchar *request = NULL;

    TextView = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "msg_entry"));
    message = get_message(TextView);
    if (message) {
        add_list_entry(client->builder, "10.01.20", "Bohdan", message, 100);
        // request = mx_auth_request(gtk_entry_get_text(phone),
        //                           gtk_entry_get_text(password));
        // mx_send_data(client->data_out, request);
        // g_free(request);
        g_free(message);
    }
    (void)button;
}
void mx_chat_contol(GtkBuilder *builder, t_client *client) {
    GtkButton *send_button = NULL;
    GtkTextView *TextView = NULL;

    send_button = GTK_BUTTON(gtk_builder_get_object(builder, "send_message"));
    TextView = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "msg_entry"));
    g_signal_connect(send_button, "clicked", G_CALLBACK(send_message), client);
}

static void add_contact(GtkButton *button, t_client *client) {
    GtkListBox *box = GTK_LIST_BOX(gtk_builder_get_object(client->builder, "listbox_rooms"));
    GtkWidget *new_client = gtk_button_new_with_label("chat");

    gtk_list_box_insert(box, new_client, -1);

    gtk_widget_show(new_client);
    (void)client;
}

static void controling(GtkBuilder *builder, t_client *client) {
    GtkWidget *chat = GTK_WIDGET(gtk_builder_get_object(builder, "chat"));
    GtkButton *create_contact = GTK_BUTTON(gtk_builder_get_object(builder, "btn_add_room"));

    g_signal_connect(create_contact, "clicked", G_CALLBACK(add_contact), client);

    mx_chat_contol(builder, client);
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
