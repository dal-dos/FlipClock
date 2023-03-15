// time_utils.c
#include "time_utils.h"

struct tm Time_get_time_zone(int offset) {
    time_t now;
    struct tm ptm;

    time(&now);
    ptm = *gmtime(&now);

    int new_hours = ptm.tm_hour + offset;
    if (new_hours < 0) {
        new_hours = 24 + new_hours;
    } else if (new_hours >= 24) {
        new_hours = new_hours - 24;
    }
    ptm.tm_hour = new_hours;

    return ptm;
}