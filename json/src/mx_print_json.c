#include "mx_json.h"

void mx_print_json(gchar *data) {
    gchar *str = json_parser_get_root(mx_str_to_json(data));

    g_print("%s", str);
}
