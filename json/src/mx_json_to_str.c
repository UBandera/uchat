#include "mx_json.h"

gchar *mx_json_to_str(JsonNode * root) {
    JsonGenerator *generator = json_generator_new();
    gchar *str = NULL;

    json_generator_set_root(generator, root);
    json_generator_set_pretty(generator, TRUE);
    str = json_generator_to_data(generator, NULL);
    json_node_free(root);
    g_object_unref(generator);
    return str;
}
