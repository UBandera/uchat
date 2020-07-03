#ifndef JSON_H
#define JSON_H

#include <stdlib.h>
#include <glib-object.h>
#include <json-glib/json-glib.h>

typedef enum s_request_types {
    RQ_SIGN_IN,
    RQ_SIGN_UP,
    RQ_LOG_OUT,
}            e_request_types;

JsonBuilder *mx_init_json(gint request_type, gchar **data);
gchar *mx_json_to_str(JsonNode * root);
JsonParser *mx_str_to_json(gchar *data);
void mx_print_json(gchar *data);

#endif /* end of include guard: JSON_H */
