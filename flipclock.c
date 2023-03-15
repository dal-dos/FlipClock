// flipclock.c
// main file to run clock alarm application
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "src/accelerometer.h"
#include "src/utils.h"
#include "src/clock.h"

#define MAX_FLIPS 5
#define MIN_FLIPS 2

Accel_direction flip_values[MAX_FLIPS] = {NONE};

static int generate_flips(void) {
    srand(time(NULL));
    int flip_count = MIN_FLIPS + rand() % (MAX_FLIPS+1 - MIN_FLIPS);
    for (int i = 0; i < flip_count; i++) {
        while (1) {
            if (i == 0) {
                Accel_direction dir = rand() % (LENGTH-1);
                if (dir != Z) { // don't select the starting position as the first flip
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
    return flip_count;
}

int main(int argc, char *argv[]) {
    Accel_start_thread();
    //Clock_start_thread();
    //Clock_set_manual_time(8, 59);
    while(1) {
        int* accel_values = Accel_get_values();
        printf("[%d], x: %d, y: %d, z: %d\n", Accel_get_recent_trig(), accel_values[0], accel_values[1], accel_values[2]);

        Utils_sleep_for_ms(200);
    }
    Accel_stop_thread();
    //Clock_stop_thread();
}
