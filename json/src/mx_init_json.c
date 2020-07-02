#include "mx_json.h"

JsonBuilder *mx_init_json(gint request_type, gchar **data) {
    JsonBuilder *builder = json_builder_new();

    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "request_type");
    json_builder_add_int_value(builder, RQ_SIGN_IN);

    json_builder_set_member_name(builder, "data");

    json_builder_begin_array(builder);
    json_builder_begin_object(builder);
    for (gint i = 0; data[i]; i++) {
        json_builder_set_member_name(builder, "field");
        json_builder_add_string_value(builder, data[i]);
    }
    json_builder_end_object(builder);
    json_builder_end_array(builder);

    json_builder_end_object(builder);
    // g_object_unref(builder);
}
