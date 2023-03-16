// flipclock.c
// main file to run clock alarm application
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "src/accelerometer.h"
#include "src/utils.h"
#include "src/clock.h"
#include "src/flip_game.h"
#include "src/alarm.h"

int main(int argc, char *argv[]) {
    Accel_start_thread();
    Clock_start_thread();
    Alarm_start_thread();
    Clock_set_manual_time(11, 21);
    Alarm_set_trigger_time(11, 22);
    while(1) {
        if (Alarm_get_triggered()) {
            Flip_start_game(60*1000);
            Alarm_dismiss();
        }
        Utils_sleep_for_ms(500);
    }
    Accel_stop_thread();
    Clock_stop_thread();
    Alarm_stop_thread();
}
