#include "client.h"

    GtkWidget *chatWindow;
    GtkWidget *sendEntry, *sendButton;
    GtkWidget *statusLabel;
    GtkWidget *messagesTreeView;
    GtkWidget *TextView;
    GtkWidget *Contacts;
    GtkAdjustment *vAdjust;
    GtkScrolledWindow *scrolledWindow;
    GtkListStore *messagesListStore;
    pthread_t watcher;

static GtkBuilder *mx_init_window() {
    GtkBuilder *builder;
    
    gtk_init(NULL, NULL);
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "gui(backup).glade", NULL);
    gtk_builder_connect_signals(builder, builder);
    return builder;
}

static void send_messege() {
    GtkTextIter start, end;
    GtkTextBuffer *buffer = gtk_text_view_get_buffer (TextView);
    gchar *text;

    gtk_text_buffer_get_bounds (buffer, &start, &end);
    text = gtk_text_buffer_get_text (buffer, &start, &end, FALSE);
    printf("%s", text);

    GtkWidget *button;
    // GtkWidget *row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    // GtkWidget *row1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    
    // GtkWidget *new_button = gtk_button_new_with_label(text);
    // gtk_widget_set_hexpand(new_button, TRUE);
    // gtk_widget_set_halign(new_button, GTK_ALIGN_END);
    // gtk_widget_set_valign(new_button, GTK_ALIGN_CENTER);
    // gtk_widget_set_size_request(new_button, 300, 5);
    // gtk_container_add(GTK_CONTAINER(row), new_button);

    // gtk_widget_hide(messagesTreeView);
    // exit(0);
}

// void *watcher_thread(void *param)
// {
//     (void) param;
//     return param;
// }

int chat_window() {
    GtkBuilder *builder = mx_init_window();

    chatWindow = GTK_WIDGET(gtk_builder_get_object(builder,"chatWindow"));

    sendButton = GTK_WIDGET(gtk_builder_get_object(builder,"sendButton"));
    sendEntry = GTK_WIDGET(gtk_builder_get_object(builder,"sendEntry"));
    Contacts = GTK_WIDGET(gtk_builder_get_object(builder,"listbox_rooms"));
    g_signal_connect(G_OBJECT(sendEntry),"activate", G_CALLBACK(send_messege) ,NULL);
    g_signal_connect(G_OBJECT(sendButton),"clicked", G_CALLBACK(send_messege) ,NULL);
    TextView = GTK_WIDGET(gtk_builder_get_object(builder,"msg_entry"));
    messagesTreeView = GTK_WIDGET(gtk_builder_get_object(builder,"messagesTreeView"));
    messagesListStore = GTK_LIST_STORE(gtk_builder_get_object(builder,"messagesListStore"));
    scrolledWindow = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder,"scrolledWindow"));
    vAdjust = gtk_scrolled_window_get_vadjustment(scrolledWindow);

    gtk_main();
    // pthread_create(&watcher, 0, watcher_thread, 0);
    // mx_widget_set_visibility(GTK_WIDGET(dialog_auth), TRUE);
    // gtk_widget_show_all("dialog_auth");
    
}
