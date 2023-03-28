// request_handler.c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// add any modules needing to be accessed through the API here
#include "request_handler.h"
#include "../src/alarm.h"
#include "../src/clock.h"

#define MSG_MAX_LEN 1500

static bool flag = true;

void Handler_make_request(char* request, char* buff) {
    snprintf(buff, MSG_MAX_LEN-1, "success=false"); // default response
    if (strncmp(request, "stop", strlen("stop")) == 0) {
        flag = false;
        snprintf(buff, MSG_MAX_LEN-1, "stop=1");
    } else if (strncmp(request, "get/", strlen("get/")) == 0) {
        char* req = request+strlen("get/");
        if (strncmp(req, "clock/", strlen("clock/")) == 0) {
            char* base = "clock/";
            req = req+strlen(base);
            if (strncmp(req, "time_zone", strlen("time_zone")) == 0) {
                int time_zone = Clock_get_time_zone();
                if (!Clock_get_automatic_time()) {
                    snprintf(buff, MSG_MAX_LEN-1,
                    "%stime_zone={\"hours\":'-'}",
                    base);
                } else {
                    snprintf(buff, MSG_MAX_LEN-1,
                        "%stime_zone={\"hours\":%d}",
                        base, time_zone);
                }
            } else if (strncmp(req, "time", strlen("time")) == 0) {
                clock_time c_t = Clock_get_time();
                snprintf(buff, MSG_MAX_LEN-1,
                    "%stime={\"hours\":%d,\"minutes\":%d,\"seconds\":%d}",
                    base, c_t.hours, c_t.minutes, c_t.seconds);
            }
        } else if (strncmp(req, "alarm/", strlen("alarm/")) == 0) {
            char* base = "alarm/";
            req = req+strlen(base);
            if (strncmp(req, "timeout", strlen("timeout")) == 0) {
                int timeout = Alarm_get_timeout()/1000/60; // convert to minutes
                snprintf(buff, MSG_MAX_LEN-1,
                    "%stimeout={\"minutes\":%d}",
                    base, timeout);
            } else if (strncmp(req, "time", strlen("time")) == 0) {
                clock_time t = Alarm_get_trigger_time();
                snprintf(buff, MSG_MAX_LEN-1,
                    "%stime={\"hours\":%d,\"minutes\":%d}",
                    base, t.hours, t.minutes);
            }
        }
    } else if (strncmp(request, "set/", strlen("set/")) == 0) { // SET
        char* req = request+strlen("set/");
        if (strncmp(req, "clock/", strlen("clock/")) == 0) {
            char* base = "clock/";
            req = req+strlen(base);
            if (strncmp(req, "time_zone=", strlen("time_zone=")) == 0) {
                Clock_set_time_zone((int)strtol(req+strlen("time_zone="), (char**)NULL, 10));

                int time_zone = Clock_get_time_zone();
                snprintf(buff, MSG_MAX_LEN-1,
                    "%stime_zone={\"hours\":%d}",
                    base, time_zone);
            } else if (strncmp(req, "time=", strlen("time=")) == 0) {
                char* value = req+strlen("time=");
                if (value[0] == '{' && value[strlen(value)-1] == '}') {
                    char *split = strtok(value+1, ",");
                    int hours = (int)strtol(split, (char**)NULL, 10);
                    split = strtok (NULL, "}");
                    int minutes = (int)strtol(split, (char**)NULL, 10);
                    
                    Clock_set_manual_time(hours, minutes);

                    clock_time c_t = Clock_get_time();
                    snprintf(buff, MSG_MAX_LEN-1,
                        "%stime={\"hours\":%d,\"minutes\":%d,\"seconds\":%d}",
                        base, c_t.hours, c_t.minutes, c_t.seconds);
                }
            }
        } else if (strncmp(req, "alarm/", strlen("alarm/")) == 0) {
            char* base = "alarm/";
            req = req+strlen(base);
            if (strncmp(req, "timeout=", strlen("timeout=")) == 0) {
                Alarm_set_timeout((int)strtol(req+strlen("timeout="), (char**)NULL, 10));

                int timeout = Alarm_get_timeout()/1000/60; // convert to minutes
                snprintf(buff, MSG_MAX_LEN-1,
                    "%stimeout={\"minutes\":%d}",
                    base, timeout);
            } else if (strncmp(req, "time=", strlen("time=")) == 0) {
                char* value = req+strlen("time=");
                if (value[0] == '{' && value[strlen(value)-1] == '}') {
                    char *split = strtok(value+1, ",");
                    int hours = (int)strtol(split, (char**)NULL, 10);
                    split = strtok (NULL, "}");
                    int minutes = (int)strtol(split, (char**)NULL, 10);
                    
                    Alarm_set_trigger_time(hours, minutes);

                    clock_time t = Alarm_get_trigger_time();
                    snprintf(buff, MSG_MAX_LEN-1,
                        "%stime={\"hours\":%d,\"minutes\":%d}",
                        base, t.hours, t.minutes);
                }
            }
        }
    }
}

bool* Handler_get_flag() {
    return &flag;
}