#include "client.h"


void mx_init_handlers(t_client *client) {
    client->response_handler[RS_SIGN_IN] = mx_sign_in_response;
    client->response_handler[RS_SIGN_UP] = mx_sign_up_response;
    client->response_handler[RS_SIGN_OUT] = mx_sign_up_response;
    client->response_handler[RQ_CONTACT_LIST] = mx_contact_list_response;
    client->response_handler[RQ_CHAT_DATA] = mx_chat_data_response;
    client->response_handler[RQ_PROFILE_DATA] = mx_profile_data_response;
    client->response_handler[RQ_SEND_MESSAGE] = mx_send_message_response;
}

void mx_receive_data(gchar *response, t_client *client) {
    cJSON *json = cJSON_Parse(response);
    gint response_type = cJSON_GetObjectItem(json, "type")->valueint;

    client->response_handler[response_type](json, client);
    cJSON_Delete(json);
}
