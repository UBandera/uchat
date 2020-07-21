#include "server.h"

#define USERNAME "uchat.llc@gmail.com"
#define PASSWORD "Qwerty1357"

#define FROM     "uchat.llc@gmail.com"
#define CC       "uchat.llc@gmail.com"


static void smtp_registation(CURL *curl) {
    curl_easy_setopt(curl, CURLOPT_USERNAME, USERNAME);
    curl_easy_setopt(curl, CURLOPT_PASSWORD, PASSWORD);
    curl_easy_setopt(curl, CURLOPT_URL, "smtps://smtp.gmail.com:465");
}

static void setup_email_header(CURL *curl, char *to) {
    const char **line;
    struct curl_slist *headers = NULL;
    const char *headers_text[] = {
        "Date: Tue, 22 Aug 2017 14:08:43 +0100",
        to,
        "From: " FROM " (Uchat)",
        "Cc: " CC " (Uchat)",
        "Subject: Uchat Password Reset",
        NULL
    };

    for(line = headers_text; *line; line++)
        headers = curl_slist_append(headers, *line);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
}

static void setup_email_recipients(CURL *curl, struct curl_slist *recipients,
                                   char *to) {
    char to_user[1024];

    snprintf(to_user, sizeof(to_user), "%s%s", "To: ", to);
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, FROM);
    recipients = curl_slist_append(recipients, to);
    recipients = curl_slist_append(recipients, CC);
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
    setup_email_header(curl, to_user);
}

static curl_mime *setup_email_body(CURL *curl, struct curl_slist *slist,
                                   char *data) {
    curl_mime *mime;
    curl_mime *alt;
    curl_mimepart *part;

    mime = curl_mime_init(curl);
    alt = curl_mime_init(curl);
    part = curl_mime_addpart(alt);
    curl_mime_data(part, data, CURL_ZERO_TERMINATED);
    curl_mime_type(part, "text/html");
    part = curl_mime_addpart(mime);
    curl_mime_subparts(part, alt);
    curl_mime_type(part, "multipart/alternative");
    slist = curl_slist_append(NULL, "Content-Disposition: inline");
    curl_mime_headers(part, slist, 1);
    curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
    return mime;
}


int mx_send_mail(char *receiver, char *body) {
    CURL *curl;
    CURLcode res = CURLE_OK;

    if ((curl = curl_easy_init())) {
        struct curl_slist *headers = NULL;
        curl_mime *mime;

        smtp_registation(curl);
        setup_email_recipients(curl, headers, receiver);
        mime = setup_email_body(curl, headers, body);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        curl_mime_free(mime);
    }
    return (int)res;
}

// int main() {
//     gchar *password = mx_generate_password();
//     gchar *body = mx_recovery_body("ARTEM", password);
//
//     mx_send_mail("shemedvedd@gmail.com", body);
//     g_free(body);
//     g_free(password);
//     return 0;
// }
