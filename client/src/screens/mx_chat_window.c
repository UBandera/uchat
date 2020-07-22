#include "client.h"

    GtkWidget *chatWindow;
    GtkWidget *sendButton;
    GtkWidget *sendEntry;
    GtkWidget *ContactButton;
    GtkWidget *messagesTreeView;
    GtkWidget *TextView;
    GtkWidget *Contacts;
    GtkAdjustment *vAdjust;
    GtkScrolledWindow *scrolledWindow;
    GtkListStore *messagesListStore;
    GtkListBox *Clients;

static GtkBuilder *mx_init_window() {
    GtkBuilder *builder;
    
    gtk_init(NULL, NULL);
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "gui(backup).glade", NULL);
    gtk_builder_connect_signals(builder, builder);
    return builder;
}

void sleep_ms(int milliseconds)
{
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
}

void add_list_entry(const char *t, const char *a, const char *m, int sleep) // заносит сообщения в окно
{
    GtkTreeIter iter;
    gtk_list_store_append(GTK_LIST_STORE(messagesListStore), &iter);
    gtk_list_store_set(GTK_LIST_STORE(messagesListStore), &iter, 0, t, 1, a, 2, m, -1);
    if(sleep)
        sleep_ms(100);
    // gtk_adjustment_set_value(vAdjust, gtk_adjustment_get_upper(vAdjust) - gtk_adjustment_get_page_size(vAdjust));
}

// static void nothing() {

// }
// static void nothing() {
//     printf("Hello\n");
// }

static void clear_list_entry() {
    // GtkWidget *button;
    
    // button = gtk_button_new_with_label("Hello");
    // gtk_widget_set_hexpand(button, TRUE);
    // gtk_widget_set_halign(button, GTK_ALIGN_CENTER);
    // gtk_widget_set_valign(button, GTK_ALIGN_CENTER);
    // gtk_container_add(GTK_CONTAINER(Contacts), button);
    // g_signal_connect(button, "clicked", G_CALLBACK(nothing), NULL);
    gtk_list_store_clear (messagesListStore);
}

static void send_messege() { // отправляет сообщения
    GtkTextIter start, end;
    GtkTextBuffer *buffer = gtk_text_view_get_buffer (TextView);
    gchar *text;

    gtk_text_buffer_get_bounds (buffer, &start, &end);
    text = gtk_text_buffer_get_text (buffer, &start, &end, FALSE);
    if (strlen(text) == 0)
        return;
    add_list_entry("10.01.20", "Bohdan", text, 100);
    gtk_text_buffer_delete(buffer, &start, &end);
}

int chat_window() {
    GtkBuilder *builder = mx_init_window();

    chatWindow = GTK_WIDGET(gtk_builder_get_object(builder,"window"));

    // sendButton = GTK_WIDGET(gtk_builder_get_object(builder,"send_message"));
    // ContactButton = GTK_WIDGET(gtk_builder_get_object(builder,"btn_add_room"));
    // // g_signal_connect(G_OBJECT(ContactButton),"clicked", G_CALLBACK(clear_list_entry) ,NULL);
    // sendEntry = GTK_WIDGET(gtk_builder_get_object(builder,"sendEntry"));
    // Contacts = GTK_WIDGET(gtk_builder_get_object(builder,"listbox_rooms"));
    // scrolledWindow = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder,"scrolledWindow"));
    // // g_signal_connect(G_OBJECT(sendEntry),"activate", G_CALLBACK(send_messege) ,NULL);
    // // g_signal_connect(G_OBJECT(sendButton),"clicked", G_CALLBACK(send_messege) ,NULL);
    // TextView = GTK_WIDGET(gtk_builder_get_object(builder,"msg_entry"));
    // messagesTreeView = GTK_WIDGET(gtk_builder_get_object(builder,"messagesTreeView"));
    // messagesListStore = GTK_LIST_STORE(gtk_builder_get_object(builder,"messagesListStore"));
    // scrolledWindow = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder,"scrolledWindow"));
    // vAdjust = gtk_scrolled_window_get_vadjustment(scrolledWindow);

    // Clients = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder,"scrolledWindow"));
    gtk_main();
    printf("after gtk_main\n");
}
