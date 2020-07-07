#ifndef JSON_H
#define JSON_H

#include <stdlib.h>
#include <glib-object.h>
#include <json-glib/json-glib.h>
#include "cJSON.h"

typedef enum s_request_types {
    RQ_SIGN_IN,
    RQ_SIGN_UP,
    RQ_SIGN_OUT,
    RQ_CONTACT_LIST,
    RQ_CHAT_DATA,
    RQ_PROFILE_DATA,
    RQ_SEND_MESSAGE,
}            e_request_types;


GHashTable *mx_json_parser(gchar *data);

#endif /* end of include guard: JSON_H */
