// alarm.h
// module that triggers an alarm at the set time
#ifndef _ALARM_H_
#define _ALARM_H_

#include <stdbool.h>

#include "clock.h"

// start/stop the alarm thread
void Alarm_start_thread(void);
void Alarm_stop_thread(void);

// set the alarm trigger time and arms the alarm clock
void Alarm_set_trigger_time(unsigned int hours, unsigned int minutes);

// set the time limit before the alarm self-dismisses
void Alarm_set_timeout(unsigned long long dismiss_after);

// dismisses the alarm
void Alarm_dismiss();

clock_time Alarm_get_trigger_time(void);

unsigned long long Alarm_get_timeout(void);

bool Alarm_get_triggered(void);

#endif
