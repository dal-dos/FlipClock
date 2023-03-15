// time_utils.h
// module containing helper functions relating to time
#ifndef _TIME_UTILS_H_
#define _TIME_UTILS_H_

#include <time.h>

// Returns the time of the specified time zone
struct tm Time_get_time_zone(int offset);

#endif
