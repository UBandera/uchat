#include "mx_json.h"

void mx_print_json(JsonNode *json) {
    gchar *str = json_to_string(json, TRUE);

    g_print("%s", str);
    g_free(str);
}
