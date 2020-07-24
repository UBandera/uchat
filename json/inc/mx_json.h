#ifndef JSON_H
#define JSON_H

#include <stdlib.h>
#include <glib-object.h>
#include "cJSON.h"

enum e_request_types {
    RQ_GENERATE_PASS = 0,
    RQ_AUTH = 1,
    RQ_SET_UP_PROFILE = 2,
    RQ_SEND_MESSAGE,
    RQ_CHAT_DATA,
    //
    RQ_RECOVERY_PASSWD,
    RQ_SIGN_IN,
    RQ_SIGN_UP,
    RQ_SIGN_OUT,
    RQ_CONTACT_LIST,
    RQ_PROFILE_DATA,
    RQ_SIZE
};

enum e_response_types {
    RS_PASSWORD_SENT,
    RS_SIGN_UP,
    RS_VALID,
    RS_SEND_MESSAGE,
    RS_GET_CHAT_HISTORY,
    //
    // RS_SIGN_IN,
    // RS_SIGN_UP,
    // RS_SIGN_OUT,
    // RS_CONTACT_LIST,
    // RS_CHAT_DATA,
    // RS_PROFILE_DATA,
    // RS_SEND_MESSAGE,

};

enum e_response_errors {
    ER_SENT_PASS = 25,
    ER_PASS = 26,
//
    // ER_SIGN_IN = -1,
    // ER_SIGN_UP = -2,
    // ER_SIGN_OUT = -3,
    // ER_CONTACT_LIST = -4,
    // ER_CHAT_DATA = -5,
    // ER_PROFILE_DATA = -6,
    // ER_SEND_MESSAGE = -7,
};



#endif /* end of include guard: JSON_H */
