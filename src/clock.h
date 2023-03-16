// clock.h
// module to display the time on a display
#ifndef _CLOCK_H_
#define _CLOCK_H_

typedef struct _clock_time {
    int hours, minutes, seconds;
} clock_time;

// start/stop the thread handling the clock display
void Clock_start_thread(void);
void Clock_stop_thread(void);

// sets a manual clock time
void Clock_set_manual_time(unsigned int hours, unsigned int minutes);

// sets the automatic time based on time zone
void Clock_set_time_zone(int time_zone);

// returns the current time as a clock_time struct
clock_time Clock_get_time(void);

#endif
