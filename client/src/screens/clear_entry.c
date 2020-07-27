#include "client.h"

void mx_clear_entry(GtkBuilder *builder, const gchar *entry_name) {
    GtkEntry *entry = NULL;
    GtkEntryBuffer* buf = NULL;

    entry = GTK_ENTRY(gtk_builder_get_object(builder, entry_name));
    buf = gtk_entry_get_buffer(entry);
    gtk_entry_buffer_set_text(buf, "\0", -1 );
}
