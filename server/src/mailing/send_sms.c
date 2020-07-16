#include "server.h"

#define MX_TEMPLATE_SIZE 10000
#define MX_MESSAGE_SIZE 42

#define MX_ACCOUNT_SID "AC6cf27775aa0531c6c4f1460b571e1475"
#define MX_AUTH_TOKEN "54129a35ab9e3af9cfc8df8efe8f4ebf"
#define MX_FROM_NUMBER "+12019285337"
#define MX_MESSAGE "Your Uchat verification code is: "

static void form_message(CURL *curl, char *to_number, char *password) {
    char url[MX_TEMPLATE_SIZE];
    char parameters[MX_TEMPLATE_SIZE];
    char message[MX_MESSAGE_SIZE];

    snprintf(url, sizeof(url), "%s%s%s",
             "https://api.twilio.com/2010-04-01/Accounts/",
             MX_ACCOUNT_SID, "/Messages");
    snprintf(message, sizeof(message), "%s%s", MX_MESSAGE, password);
    snprintf(parameters, sizeof(parameters), "%s%s%s%s%s%s", "To=",
             to_number, "&From=", MX_FROM_NUMBER, "&Body=", message);
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, parameters);
    curl_easy_setopt(curl, CURLOPT_USERNAME, MX_ACCOUNT_SID);
    curl_easy_setopt(curl, CURLOPT_PASSWORD, MX_AUTH_TOKEN);
}

static int error_handling(CURL *curl, CURLcode res) {
    long http = 0;

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http);
    if (res != CURLE_OK) {
        fprintf(stderr, "SMS send failed: %s.\n", curl_easy_strerror(res));
        return -1;
    }
    else if (http != 200 && http != 201) {
        fprintf(stderr, "SMS send failed, HTTP Status Code: %ld.\n", http);
        return -1;
    }
    return 0;
}

int mx_send_sms(char *to_number, char *password) {
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    form_message(curl, to_number, password);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (error_handling(curl, res)) {
        return -1;
    }
    else {
        fprintf(stderr, "SMS sent successfully!\n");
        return 0;
    }
}
