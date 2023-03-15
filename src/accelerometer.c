// accelerometer.c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>

#include "accelerometer.h"
#include "utils.h"

#define REG_OUT_X_L 0x28
#define REG_OUT_X_H 0x29
#define REG_OUT_Y_L 0x2A
#define REG_OUT_Y_H 0x2B
#define REG_OUT_Z_L 0x2C
#define REG_OUT_Z_H 0x2D

#define I2C_DEVICE_ADDRESS 0x18

#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"

#define BUFF_LEN 6

static pthread_t accel_thread;
static bool running; // thread start/stop flag

int accel_values[3] = {0};

typedef enum {
    X,
    N_X,
    Y,
    N_Y,
    Z,
    N_Z,
    NONE,
} Accel_direction;

// read accelerometer values
static void* accel_thread_helper(void* arg) {
    int i2cFileDesc_BUS1 = Utils_initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);
    Utils_writeI2cReg(i2cFileDesc_BUS1, 0x20, 0x27);

    Accel_direction prev_trig = NONE;
    while (running) {
        long long debounce_time = 25;
        unsigned char buff[BUFF_LEN];
        Utils_readMultipleI2cReg(i2cFileDesc_BUS1, 0x28 + 0x80, buff, BUFF_LEN);
        int16_t x = (buff[1] << 8) | (buff[0]);
        int16_t y = (buff[3] << 8) | (buff[2]);
        int16_t z = (buff[5] << 8) | (buff[4]);
        //z -= 15000; // gravity offset

        int new_values[3] = {x, y, z};
        int threshold = 10000;
        Accel_direction max_dir = NONE;
        if (abs(x) >= abs(y)) {
            if (abs(x) >= abs(z)) {
                max_dir = X;
            } else { max_dir = Z; }
        } else if (abs(y) >= abs(z)) {
            max_dir = Y;
        } else {
            max_dir = Z;
        }

        if ((x >= threshold) && max_dir == X) {
            if (prev_trig == X) {
                debounce_time = 200;
                printf("FLIPPED X\n");
            }
            prev_trig = X;
        } else if ((x <= -threshold) && max_dir == X) {
            if (prev_trig == N_X) {
                debounce_time = 200;
                printf("FLIPPED -X\n");
            }
            prev_trig = N_X;
        } else if ((y >= threshold) && max_dir == Y) {
            if (prev_trig == Y) {
                debounce_time = 200;
                printf("FLIPPED Y\n");
            }
            prev_trig = Y;
        } else if ((y <= -threshold) && max_dir == Y) {
            if (prev_trig == N_Y) {
                debounce_time = 200;
                printf("FLIPPED -Y\n");
            }
            prev_trig = N_Y;
        } else if ((z >= threshold) && max_dir == Z) {
            if (prev_trig == Z) {
                debounce_time = 200;
                printf("FLIPPED Z\n");
            }
            prev_trig = Z;
        } else if ((z <= -threshold) && max_dir == Z) {
            if (prev_trig == N_Z) {
                debounce_time = 200;
                printf("FLIPPED -Z\n");
            }
            prev_trig = N_Z;
        } else {
            prev_trig = NONE;
        }
        memcpy(accel_values, new_values, sizeof(new_values));
        //printf("[x] %d, [y] %d, [z] %d\n", x, y, z);

        Utils_sleep_for_ms(debounce_time);
    }
    return NULL;
}


void Accel_start_thread(void) {
    if (running != true) {
        Utils_run_command("config-pin P9_18 i2c");
        Utils_run_command("config-pin P9_17 i2c");

        running = true;
        pthread_create(&accel_thread, NULL, accel_thread_helper, NULL);
    }
}

void Accel_stop_thread(void) {
    running = false;
    pthread_join(accel_thread, NULL);
}

int* Accel_get_values(void) {
    return accel_values;
}