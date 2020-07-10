#include "client.h"

void mx_notify(GApplication *application) {
    GNotification *notification;

    notification = g_notification_new("Lunch is ready");
    g_notification_set_body(notification, "Today we have pancakes and salad, and fruit and cake for dessert");
    g_application_send_notification(application, "lunch-is-ready", notification);
    g_object_unref (notification);
}
