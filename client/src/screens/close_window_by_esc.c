#include "client.h"

gboolean mx_close_window_by_esc(GtkWidget *widget, GdkEventKey *event,
                                gpointer data) {
  if (event->keyval == GDK_KEY_Escape) {
      gtk_widget_hide(GTK_WIDGET(data));
      return TRUE;
  }
  return FALSE;
}
