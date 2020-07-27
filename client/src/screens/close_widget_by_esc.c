#include "client.h"

gboolean mx_close_widget(GtkWidget *widget, GdkEventKey *event,
                         GtkWidget *to_close) {
    if (event->keyval == GDK_KEY_KP_Enter) {
        gtk_widget_set_visible(to_close, FALSE);
        return TRUE;
    }
    return FALSE;
}