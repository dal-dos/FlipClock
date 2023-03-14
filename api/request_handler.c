// request_handler.c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// add any modules needing to be accessed through the API here
#include "request_handler.h"

#define MSG_MAX_LEN 1500

static bool flag = true;

void Handler_make_request(char* request, char* buff) {
    if (strncmp(request, "stop", strlen("stop")) == 0) {
            flag = false;
            snprintf(buff, MSG_MAX_LEN-1, "1");
    } else if (strncmp(request, "get ", strlen("get ")) == 0) {
        char* req = request+strlen("get ");
        if (strncmp(req, "beat", strlen("beat")) == 0) {
            snprintf(buff, MSG_MAX_LEN-1, "beat %s", "value");
        }
    } else if (strncmp(request, "set ", strlen("set ")) == 0) {
        char* req = request+strlen("set ");
        if (strncmp(req, "beat ", strlen("beat ")) == 0) {
            int beat_id = atoi(request + strlen("beat "));
            snprintf(buff, MSG_MAX_LEN-1, "1");
        } else if (strncmp(req, "volume ", strlen("volume ")) == 0) {
            int interval = 5;
            if (strncmp(req+strlen("volume "), "decrease", strlen("decrease")) == 0) {
            } else if (strncmp(req+strlen("volume "), "increase", strlen("increase")) == 0) {
            }
            snprintf(buff, MSG_MAX_LEN-1, "1");
        } else {
            snprintf(buff, MSG_MAX_LEN-1, "Unknown command.\n");
        }
    }
}

bool Handler_get_flag() {
    return flag;
}