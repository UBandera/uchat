#include "mx_json.h"

GHashTable *mx_json_parser(gchar *data) {
    JsonNode *json = json_from_string(data, NULL);
    JsonReader *reader = json_reader_new(json);
    GHashTable *request = g_hash_table_new(g_str_hash, g_str_equal);

    if (json_reader_read_element(reader, 0)) {
        gint request_type = json_reader_get_int_value(reader);
        json_reader_end_element (reader);
        g_print("request_type = %d\n", request_type);
    }
    if (json_reader_read_element(reader, 1)) {
        const gchar *jdon_data = json_reader_get_string_value(reader);
        json_reader_end_element (reader);
        g_print("data = %s\n", jdon_data);
    }
    if (json_reader_read_element(reader, 2)) {
        const gchar *jdon_data = json_reader_get_string_value(reader);
        json_reader_end_element (reader);
        g_print("data = %s\n", jdon_data);
    }
    // for (gint i = 1; json_reader_read_element(reader, i); i++) {
    //     const gchar *jdon_data = json_reader_get_string_value (reader);
    //
    //     g_print("data = %s\n", jdon_data);
    //     json_reader_end_element (reader);
    // }
    return request;
}
