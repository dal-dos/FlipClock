// accelerometer.h
// module for reading the accelerometer
#ifndef _ACCELEROMETER_H_
#define _ACCELEROMETER_H_

// Starts/stops the thread to read and handle accelerometer values
void Accel_start_thread(void);
void Accel_stop_thread(void);

// Returns the accelerometer values as an array tuple
// [x, y, z]
int* Accel_get_values(void);
#endif