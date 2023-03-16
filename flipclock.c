// flipclock.c
// main file to run clock alarm application
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "src/accelerometer.h"
#include "src/utils.h"
#include "src/clock.h"
#include "src/flip_game.h"

int main(int argc, char *argv[]) {
    Accel_start_thread();
    //Clock_start_thread();
    //Clock_set_manual_time(8, 59);
    while(1) {
        //int* accel_values = Accel_get_values();
        //printf("[%d], x: %d, y: %d, z: %d\n", Accel_get_recent_trig(), accel_values[0], accel_values[1], accel_values[2]);
        printf("[START]\n");
        // alarm started
        Flip_start_game(60*1000);
        // stop alarm
        printf("[DONE]\n\n");

        Utils_sleep_for_ms(200);
    }
    Accel_stop_thread();
    //Clock_stop_thread();
}
