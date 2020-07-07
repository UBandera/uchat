#include "mx_client.h"

static gboolean mx_check_repeat_password(const gchar *password,
                                         const gchar *repeat_password) {
    if (g_strcmp0(password, repeat_password))
        return FALSE;
    return TRUE;
}

static gboolean mx_check_password(const gchar *password) {
    if (strlen(password) < 6) {
        g_print("The password should be 6 to 50 characters long!\n");
        return FALSE;
    }
    if (mx_match(password, MX_FORBIDEN_PATTERN, 0, 0)) {
        g_print("The password includes forbidden character!\n");
        return FALSE;
    }
    if (!mx_match(password, MX_ALLOWED_PATTERN, 0, 0)) {
        g_print("The password must contain uppercase and lowercase "
                "letters and numbers!\n");
        return FALSE;
    }
    g_print("correct\n");
    return TRUE;
}

static gboolean mx_check_login(const gchar *login) {
    if (strlen(login) < 6) {
        g_print("The login should be 6 to 48 characters long!\n");
        return FALSE;
    }
    if (mx_match(login, MX_FORBIDEN_PATTERN, 0, 0)) {
        g_print("The password includes forbidden character!\n");
        return FALSE;
    }
    return TRUE;
}

gint mx_auth_confirming(gchar *login,
                        gchar *password,
                        gchar *repeat_password) {
    if (!mx_check_login(login)) {
        return MX_LOGIN_ERROR;
    }
    if (!mx_check_password(password)) {
        return MX_PASSWORD_ERROR;
    }
    if (repeat_password) {
        if (!mx_check_repeat_password(password, repeat_password))
            return MX_REPEAT_PASSWORD_ERROR;
    }
    return MX_VALID;
}
