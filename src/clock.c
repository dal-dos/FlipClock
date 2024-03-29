// clock.c
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

#include "clock.h"
#include "time_utils.h"
#include "utils.h"

#define PDT (-7)

static pthread_t clock_thread;
static bool running; // thread start/stop flag

static int current_time_zone = PDT; // default pdt
static bool automatic_time = true;

static long long manual_start_time = 0;

static int current_hours = 0;
static int current_minutes = 0;
static int current_seconds = 0;

static void calculate_manual_time(long long start_time, long long current_time);

static void* clock_thread_helper(void* arg) {
    int count = 0;
    while (running) {
        count++;
        if (automatic_time) {
            struct tm ptm = Time_get_time_zone(current_time_zone);
            current_hours = ptm.tm_hour;
            current_minutes = ptm.tm_min;
            current_seconds = ptm.tm_sec;
        } else {
            calculate_manual_time(manual_start_time, Utils_get_time_in_ms());
        }
        if (count%2 == 0) { // for console, print every 2 updates
            printf("Current Time: %02d:%02d:%02d\n", current_hours, current_minutes, current_seconds);
        }
        Utils_sleep_for_ms(500);
    }
    return NULL;
}

void Clock_start_thread(void) {
    if (running != true) {
        running = true;
        pthread_create(&clock_thread, NULL, clock_thread_helper, NULL);
    }
}

void Clock_stop_thread(void) {
    running = false;
    pthread_join(clock_thread, NULL);
}

void Clock_set_manual_time(unsigned int hours, unsigned int minutes) {
    long long new_hours = (hours >= 24) ? 0 : hours;
    long long new_minutes = (minutes >= 60) ? 0 : minutes;
    long long ms_per_minute = 60000;
    long long ms_per_hour = 3600000; // 60*60*1000

    manual_start_time = Utils_get_time_in_ms() - (new_hours*ms_per_hour) - (new_minutes*ms_per_minute);
    current_hours = new_hours;
    current_minutes = new_minutes;
    current_seconds = 0;
    automatic_time = false;
}

void Clock_set_time_zone(int time_zone) {
    current_time_zone = time_zone;
    automatic_time = true;
}

// converts the provided start and end times(ms) to clock hours and minutes
static void calculate_manual_time(long long start_time, long long current_time) {
    long long ms_per_minute = 60000;
    long long ms_per_hour = 3600000; // 60*60*1000
    long long ms_per_second = 1000;
    long long time_diff = current_time - start_time;

    current_hours = (time_diff/ms_per_hour)%24;
    current_minutes = ((time_diff-((time_diff/ms_per_hour)*ms_per_hour))/ms_per_minute)%60;
    current_seconds = ((time_diff-((time_diff/ms_per_hour)*ms_per_hour)-(
        ((time_diff-((time_diff/ms_per_hour)*ms_per_hour))/ms_per_minute)*ms_per_minute))/ms_per_second)%1000;
}

void Clock_set_automatic_time(bool automatic) {
    automatic_time = automatic;
}

clock_time Clock_get_time(void) {
    clock_time t = { current_hours, current_minutes, current_seconds };
    return t;
}

int Clock_get_time_zone(void) {
    return current_time_zone;
}

bool Clock_get_automatic_time(void) {
    return automatic_time;
}