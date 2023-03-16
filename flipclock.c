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
#include "api/request_handler.h"
#include "api/udp_socket.h"

int main(int argc, char *argv[]) {
    Accel_start_thread();
    Clock_start_thread();
    Alarm_start_thread();
    UDP_start_listening(12345);

    Clock_set_manual_time(11, 21);
    Alarm_set_trigger_time(11, 22);
    while(Handler_get_flag()) {
        if (Alarm_get_triggered()) {
            Flip_start_game(Alarm_get_timeout());
            Alarm_dismiss();
        }
        Utils_sleep_for_ms(100);
    }
    Accel_stop_thread();
    Clock_stop_thread();
    Alarm_stop_thread();
    UDP_stop_listening();
}
