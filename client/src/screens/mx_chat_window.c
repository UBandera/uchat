#include "client.h"

    GtkWidget *chatWindow;
    GtkWidget *sendEntry, *sendButton;
    GtkWidget *statusLabel;
    GtkWidget *messagesTreeView;
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
    exit(0);
    // gchar *mess;
    // if(!gtk_widget_get_sensitive(sendButton))
    //     return;
    // mess = gtk_entry_get_text(GTK_ENTRY(sendEntry));
}

// void *watcher_thread(void *param)
// {
//     (void) param;
//     return param;
// }

int chat_window() {
    GtkBuilder *builder = mx_init_window();

    chatWindow = GTK_WIDGET(gtk_builder_get_object(builder,"chatWindow"));
    gtk_main();

    sendButton = GTK_WIDGET(gtk_builder_get_object(builder,"sendButton"));
    sendEntry = GTK_WIDGET(gtk_builder_get_object(builder,"sendEntry"));
    g_signal_connect(G_OBJECT(sendEntry),"activate", G_CALLBACK(send_messege) ,NULL);
    g_signal_connect(G_OBJECT(sendButton),"clicked", G_CALLBACK(send_messege) ,NULL);
    messagesTreeView = GTK_WIDGET(gtk_builder_get_object(builder,"messagesTreeView"));
    messagesListStore = GTK_LIST_STORE(gtk_builder_get_object(builder,"messagesListStore"));
    scrolledWindow = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder,"scrolledWindow"));
    vAdjust = gtk_scrolled_window_get_vadjustment(scrolledWindow);
    // pthread_create(&watcher, 0, watcher_thread, 0);
    // mx_widget_set_visibility(GTK_WIDGET(dialog_auth), TRUE);
    // gtk_widget_show_all("dialog_auth");
    
}
