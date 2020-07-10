#include "client.h"

#define MX_SIGN_UP_ERROR 1
#define MX_SIGN_UP_SUCCESSFULLY 0

/*
 * PURPOSE : Autogenerates function contract comments
 *  PARAMS : json - formed json receiving from server response string
 *           client - struct that contain all nessasery data (in this case
 *           GtkBuilder for showing reaction in ui).
 * RETURNS : void
 *   NOTES : -
 */
void mx_sign_up_response(cJSON *json, t_client *client) {
    gint status = cJSON_GetObjectItem(json, "status")->valueint;
    gchar *message = cJSON_GetObjectItem(json, "message")->valuestring;

    if (status == MX_SIGN_UP_ERROR) {
        // print label in ui with showing this error_message
        g_warning("%s\n", message);
    }
    else if (status == MX_SIGN_UP_SUCCESSFULLY) {
        // redirect to login_screen
        // print label in ui with showing success message

    }
    // if unknown error eccured
    else {
        // crush or printing error deppends on situation.
        g_error("Unknown error occured!");
    }
}
