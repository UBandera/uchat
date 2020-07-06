#include "mx_json.h"

static void get_data(gpointer key, gpointer value, gpointer user_data) {
    JsonBuilder *builder = (JsonBuilder *)user_data;

    json_builder_set_member_name(builder, g_strdup((gchar *)key));
    json_builder_add_string_value(builder, g_strdup((gchar *)value));
}

JsonNode *mx_init_json(gint request_type, GHashTable *data) {
    JsonBuilder *builder = json_builder_new();
    JsonNode *json = NULL;

    json_builder_begin_object(builder);
    json_builder_set_member_name(builder, "request_type");
    json_builder_add_int_value(builder, request_type);
    // json_builder_set_member_name(builder, "data");
    // json_builder_begin_object(builder);
    g_hash_table_foreach(data, get_data, builder);
    // json_builder_end_object(builder);
    json_builder_end_object(builder);
    json = json_builder_get_root(builder);
    g_object_unref(builder);

    return json;
}
