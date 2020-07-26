#include "server.h"

static gboolean json_validator(cJSON *root) {
    cJSON *phone = cJSON_GetObjectItemCaseSensitive(root, "phone");
    cJSON *token = cJSON_GetObjectItemCaseSensitive(root, "token");

    if (phone && token) {
        if (cJSON_IsString(phone) && cJSON_IsString(token))
            return TRUE;
        else
            return FALSE;
    }
    else
        return FALSE;
}

gchar *mx_add_contact_response(gint contact) {
    cJSON *json = cJSON_CreateObject();
    gchar *response = NULL;

    cJSON_AddNumberToObject(json, "response_type", RS_ADD_CONTACT);
    cJSON_AddNumberToObject(json, "contact", contact);
    cJSON_AddStringToObject(json, "message", "Contact add successfully");
    response = cJSON_PrintUnformatted(json);
    if (!response) {
        g_warning("Failed to print mx_add_contact_response.\n");
    }
    cJSON_Delete(json);
    return response;
}

gchar *mx_add_contact_run(sqlite3_stmt *stmt, gint contact) {
    sqlite3 *db = *(mx_get_db());
    gint rc = 0;

    if ((rc = sqlite3_step(stmt)) == SQLITE_DONE) {
        return mx_add_contact_response(contact);
    }
    else
        g_warning("mx_add_contact_run step rc:%d, %s\n", rc,
                  sqlite3_errmsg(db));
    if ((rc = sqlite3_finalize(stmt)) != SQLITE_OK)
        g_warning("mx_add_contact_run finalize rc:%d\n", rc);
    return NULL;
}

gint mx_add_contact_prepare(sqlite3_stmt **stmt,
                            gint contact,
                            gint user_id) {
    sqlite3 *db = *(mx_get_db());
    gchar *query = "INSERT INTO user_contact_list(user_id, contact)\
                    VALUES(?, ?);";
    gint rc = 0;
    g_message("uid:%d, cont:%d", user_id, contact);

    if ((rc = sqlite3_prepare_v2(db, query, -1, stmt, NULL)) != SQLITE_OK)
        g_warning("mx_add_contact_prepare prepare: %d, %s\n", rc,
                  sqlite3_errmsg(db));
    if ((rc = sqlite3_bind_int(*stmt, 1, user_id)) != SQLITE_OK)
        g_warning("mx_add_contact_prepare bind: user_id:%d %d\n",
                  contact, rc);
    if ((rc = sqlite3_bind_int(*stmt, 2, contact)) != SQLITE_OK)
        g_warning("mx_add_contact_prepare bind: contact:%d %d\n",
                  contact, rc);
    return rc;
}

void mx_add_contact_handler(cJSON *root, t_client *client) {
    // if (json_validator(root)) {
        gchar *token = cJSON_GetObjectItem(root, "token")->valuestring;
        gint contact = cJSON_GetObjectItem(root, "user_id")->valueint;
        sqlite3_stmt *stmt= NULL;
        gchar *response = NULL;

        if (contact == client->uid) {
            response = mx_send_error_response(ER_CONTACT_NOT_FOUND,
                                              "U can`t add yourself");
        }
        else {
            mx_add_contact_prepare(&stmt, contact, client->uid);
            response = mx_add_contact_run(stmt, contact);
        }
        mx_send_data(client->data_out, response);
        return;
        // }
        g_print("Not valid token\n");
    // }
    g_warning("Not valid request\n");
}
