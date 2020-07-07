#include "mx_client.h"

gboolean mx_match(const gchar *str,
                  const gchar *pattern,
                  gint compile_flags,
                  gint match_flag) {
    GRegex *regex;
    GMatchInfo *matchInfo;
    GError *error = NULL;
    gboolean result;

    regex = g_regex_new(pattern, 0, match_flag, &error);
    if (error)
        g_error("Reqex error: %s\n", error->message);
    else {
        g_regex_match_all(regex, str, 0, &matchInfo);
        result = g_match_info_matches(matchInfo) == TRUE;
        g_match_info_free(matchInfo);
        g_regex_unref(regex);
        return result;
    }
}
