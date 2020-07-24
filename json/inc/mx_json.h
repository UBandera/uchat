#ifndef JSON_H
#define JSON_H

#include <stdlib.h>
#include <glib-object.h>
#include "cJSON.h"

enum e_request_types {
    RQ_SIGN_IN,
    RQ_SIGN_UP,
    RQ_SEND_MESSAGE,
    RQ_CHAT_DATA,
    RQ_SIGN_OUT,
    RQ_CONTACT_LIST,
    RQ_PROFILE_DATA,
    RQ_SIZE
};

enum e_response_types {
    RS_SIGN_IN,
    RS_SIGN_UP,
    RS_SEND_MESSAGE,
    RS_GET_CHAT_HISTORY,
    RS_CHAT_DATA,
    RS_SIGN_OUT,
    RS_CONTACT_LIST,
    RS_PROFILE_DATA,
};

enum e_response_errors {
    ER_SIGN_IN = -1,
    ER_SIGN_UP = -2,
    ER_SIGN_OUT = -3,
    ER_CONTACT_LIST = -4,
    ER_CHAT_DATA = -5,
    ER_PROFILE_DATA = -6,
    ER_SEND_MESSAGE = -7,
};


GHashTable *mx_json_parser(gchar *data);

#endif /* end of include guard: JSON_H */
