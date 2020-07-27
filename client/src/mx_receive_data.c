#include "client.h"


static void auth_handlers(t_client *client) {
    client->response_handler[RS_PASSWORD_SENT] = mx_handle_password;
    client->response_handler[RS_SIGN_UP] = mx_sign_up_user;
    client->response_handler[RS_VALID] = mx_auth_validated;
    client->response_handler[RS_CONTACT_LIST] = mx_contact_list;
    client->response_handler[RS_CONTACT] = mx_get_contact;
    client->response_handler[RS_ADD_CONTACT] = mx_add_contact;
    client->response_handler[RS_SEND_MESSAGE] = mx_send_message;
    client->response_handler[RS_GET_CHAT_HISTORY] = mx_get_chat_history;
    client->response_handler[RS_SIGN_OUT] = mx_sign_out;
    client->response_handler[RS_REMOVE_CONTACT] = mx_remove_contact;

    client->response_handler[ER_SENT_PASS] = mx_sms_error;
    client->response_handler[ER_PASS] = mx_invalid_password;
    client->response_handler[ER_CONTACT_NOT_FOUND] = mx_contact_not_found;
}

void mx_init_handlers(t_client *client) {
    auth_handlers(client);
}

void mx_receive_data(gchar *response, t_client *client) {
    cJSON *json = cJSON_Parse(response);
    gint response_type = cJSON_GetObjectItem(json, "type")->valueint;

    client->response_handler[response_type](json, client);
    cJSON_Delete(json);
}
