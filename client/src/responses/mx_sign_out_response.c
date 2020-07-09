#include "mx_client.h"

#define MX_SIGN_OUT_ERROR 1
#define MX_SIGN_OUT_SUCCESSFULLY 0

/*
 * PURPOSE : Autogenerates function contract comments
 *  PARAMS : json - formed json receiving from server response string
 *           client - struct that contain all nessasery data (in this case
 *           GtkBuilder for showing reaction in ui).
 * RETURNS : void
 *   NOTES : -
 */
void mx_sign_out_response(cJSON *json, t_client *client) {
    gint status = cJSON_GetObjectItem(json, "status")->valueint;
    gchar *message = cJSON_GetObjectItem(json, "message")->valuestring;

    if (status == MX_SIGN_OUT_ERROR) { // SERVER_ERROR
        // lounch dialog window with label that contain error message
        g_warning("%s\n", message);
    }
    else if (status == MX_SIGN_OUT_SUCCESSFULLY) {
        // redirect to login_screen
    }
    // if unknown error eccured
    else {
        // crush or printing error deppends on situation.
        g_error("Unknown error occured!");
    }
}
