#include "mx_json.h"

JsonParser *mx_str_to_json(gchar *data) {
    JsonParser *parser = json_parser_new();
    // JsonReader *reader = NULL;

    json_parser_load_from_data(parser, data, -1, NULL);
    // reader = json_reader_new (json_parser_get_root (parser));
    // json_reader_read_member (reader, "url");
    // const char *url = json_reader_get_string_value (reader);
    // json_reader_end_member (reader);
    //
    // json_reader_read_member (reader, "size");
    // json_reader_read_element (reader, 0);
    // int width = json_reader_get_int_value (reader);
    // json_reader_end_element (reader);
    // json_reader_read_element (reader, 1);
    // int height = json_reader_get_int_value (reader);
    // json_reader_end_element (reader);
    // json_reader_end_member (reader);

    // g_object_unref (reader);

    // gchar *str = mx_json_to_str(json_parser_get_root(parser));

    return parser;
}
