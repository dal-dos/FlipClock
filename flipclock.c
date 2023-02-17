#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h>
#include "utility.h"

int alarm_hour, alarm_minute;

void RingAlarm(int hour, int minute) {
    printf("ALARM!!! %02d:%02d\n", hour, minute);
}

int main(int argc, char *argv[]) {
    time_t current_time;
    struct tm *local_time;
    struct timespec ts;

    printf("Enter alarm time (hour:minute): ");
    scanf("%d:%d", &alarm_hour, &alarm_minute);

    while (1) {
        current_time = time(NULL);
        local_time = localtime(&current_time);
        printf("Current time: %02d:%02d:%02d\n", local_time->tm_hour, local_time->tm_min, local_time->tm_sec);

        // check for alarm time
        if (local_time->tm_hour == alarm_hour && local_time->tm_min == alarm_minute) {
            RingAlarm(alarm_hour, alarm_minute);
        }

        // update tm_sec to the next second and sleep until the next second
        current_time = time(NULL);
        local_time = localtime(&current_time);
        local_time->tm_sec++; // increment to the next second
        ts.tv_nsec = 1000000000 - local_time->tm_nsec; // calculate nanoseconds until the next second
        nanosleep(&ts, NULL);
    }

    return 0;
}
