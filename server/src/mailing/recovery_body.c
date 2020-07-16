#include "server.h"

char *mx_recovery_body(char *user_name) {
    char *body = (char *)malloc(sizeof(char) * 1600);
    char *password = mx_generate_password();

    sprintf(body, "<html><body>\r\n<p><strong>%s,</p>\r\n \
         <p>Someone requested that the password for your Uchat account be \
         reseted.</p><p>Please, use this temporary password to change \
         your one.</p><p>Password: <code><strong>%s</strong></code>.</p> \
         <p><em>This password is good until today at midnight and then \
         You must generate a new one.</em></p></body></html>\r\n",user_name, password);
    // mx_strdel(&username);
    // mx_strdel(&password);
    return body;
}
