// flip_game.c
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#include "flip_game.h"
#include "accelerometer.h"
#include "utils.h"

// static pthread_t flip_thread;
// static bool running = false;

#define MAX_FLIPS 6
#define MIN_FLIPS 3

// static void* flip_thread_helper(void* arg) {

// }

// void Flip_start_thread(void) {
//     if (running != true) {
//         running = true;
//         pthread_create(&flip_thread, NULL, flip_thread_helper, NULL);
//     }
// }

// void Flip_stop_thread(void) {
//     running = false;
//     pthread_join(flip_thread, NULL);
// }

bool Flip_start_game(long long timeout, bool* early_stop) {
    Accel_direction flip_values[MAX_FLIPS] = {NONE};
    srand(time(NULL));
    // generate values
    int flip_count = MIN_FLIPS + rand() % (MAX_FLIPS+1 - MIN_FLIPS);
    for (int i = 0; i < flip_count; i++) {
        while (1) {
            if (i == 0) {
                Accel_direction dir = rand() % (LENGTH-1);
                if (dir != Accel_get_recent_trig()) { // don't select the starting position as the first flip
                    flip_values[i] = dir;
                    break;
                }
            } else {
                Accel_direction dir = rand() % (LENGTH-1);
                if (dir != flip_values[i-1]) { // don't allow dupes
                    flip_values[i] = dir;
                    break;
                }
            }
        }
    }

    long long start_time = Utils_get_time_in_ms();
    // start checking
    for (int i = 0; i < flip_count; i++) {
        long long time_elapsed = 0;
        // replace with LED indicators
        switch(flip_values[i]) {
            case 0:
                printf("LEFT\n");
                break;
            case 1:
                printf("RIGHT\n");
                break;
            case 2:
                printf("BACK\n");
                break;
            case 3:
                printf("FRONT\n");
                break;
            case 4:
                printf("TOP\n");
                break;
            case 5:
                printf("BOTTOM\n");
                break;
            case 6:
                break;
            case 7:
                break;
        }
        while (Accel_get_recent_trig() != flip_values[i]) {
            time_elapsed = Utils_get_time_in_ms()-start_time;
            if (time_elapsed >= timeout) { break; }
            if (!(*early_stop)) { break; }
        }
        if (time_elapsed >= timeout) { break; }
        if (!(*early_stop)) { break; }
        printf("*\n");
        Utils_sleep_for_ms(1000);
    }
    return true;
}