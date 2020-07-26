#include "client.h"

gchar *mx_get_message_from_entry(GtkWidget *text_view) {
    GtkTextIter start;
    GtkTextIter end;
    GtkTextBuffer *buffer = gtk_text_view_get_buffer((GtkTextView *)text_view);
    gchar *text;

    gtk_text_buffer_get_bounds(buffer, &start, &end);
    text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
    return text;
}

static void send_message(GtkButton *button, GtkWidget *text_view) {
    t_client *client = *mx_get_client();
    gchar *text = mx_get_message_from_entry(text_view);

    if (strlen(text) > 0) {
        gchar *request = mx_send_message(client->chat_with,
                                         client->token, text);

        mx_send_data(client->data_out, request);
        g_free(request);
    }
    g_free(text);
}

static gboolean mx_close_chat(GtkWidget *widget, GdkEventKey *event,
                              GtkWidget *to_close) {
    if (event->keyval == GDK_KEY_Escape) {
        gtk_widget_set_visible(to_close, FALSE);
        return TRUE;
    }
    return FALSE;
}

void mx_chat_control(GtkBuilder *builder, t_client *client) {
    GtkWidget *text_view = NULL;
    GtkButton *send_btn = NULL;

    client->chat_with = -1;
    client->text_view = GTK_WIDGET(gtk_builder_get_object(builder,
                                                          "message_entry"));
    client->chat_box = GTK_WIDGET(gtk_builder_get_object(builder, "chat_box"));
    client->chat = GTK_LIST_BOX(gtk_builder_get_object(builder, "message_box"));
    send_btn = GTK_BUTTON(gtk_builder_get_object(builder, "send_btn"));
    text_view = GTK_WIDGET(gtk_builder_get_object(builder, "message_entry"));
    g_signal_connect(send_btn, "clicked", G_CALLBACK(send_message), text_view);
    g_signal_connect(client->chat, "key_press_event",
                     G_CALLBACK(mx_close_chat), client->chat_box);
}
