// seg_display.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#include "seg_display.h"
#include "clock.h"
#include "utils.h"

#define I2C_DEVICE_ADDRESS 0x70
#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"

#define REG_OUTA1 0x00
#define REG_OUTA2 0x01
#define REG_OUTB1 0x02
#define REG_OUTB2 0x03
#define REG_OUTC1 0x04
#define REG_OUTC2 0x05
#define REG_OUTD1 0x06
#define REG_OUTD2 0x07

#define REG_SYSTEM_ON 0x21
#define REG_DISPLAY_ON 0x81
#define REG_DISPLAY_OFF 0x80

static int i2cFileDesc_BUS1;

static pthread_t seg_display_thread;

static bool running = false;

const char INT_HEX_MAP[10] = { 0x3F, 0x06, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0x07, 0xFF, 0xEF };

static void* seg_display_thread_helper(void* arg) {
    const int max_digits = 2;
    while (running) {
        clock_time current_time = Clock_get_time();
        char hours_buffer[max_digits+1];
        char minutes_buffer[max_digits+1];
        sprintf(hours_buffer, "%02d", current_time.hours);
        sprintf(minutes_buffer, "%02d", current_time.minutes);
        int A1 = hours_buffer[0] - '0';
        int B1 = hours_buffer[1] - '0';
        int C1 = minutes_buffer[0] - '0';
        int D1 = minutes_buffer[1] - '0';

        //printf("%d%d:%d%d\n", A1, B1, C1, D1);

        Utils_writeI2cReg(i2cFileDesc_BUS1, REG_OUTA1, INT_HEX_MAP[A1]);
        Utils_writeI2cReg(i2cFileDesc_BUS1, REG_OUTB1, INT_HEX_MAP[B1]);
        Utils_writeI2cReg(i2cFileDesc_BUS1, REG_OUTC1, INT_HEX_MAP[C1]);
        Utils_writeI2cReg(i2cFileDesc_BUS1, REG_OUTD1, INT_HEX_MAP[D1]);
        Utils_sleep_for_ms(1000);
    }
    return NULL;
}

void Seg_start_thread(void) {
    if (running != true) {
        running = true;
        Utils_run_command("config-pin P9_18 i2c");
        Utils_run_command("config-pin P9_17 i2c");
        i2cFileDesc_BUS1 = Utils_initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);

        // turn on sys register first
        Utils_writeI2cReg(i2cFileDesc_BUS1, REG_SYSTEM_ON, 0xFF);
        Utils_writeI2cReg(i2cFileDesc_BUS1, REG_DISPLAY_ON, 0xFF);

        // clear digits
        Utils_writeI2cReg(i2cFileDesc_BUS1, REG_OUTA2, 0x00);
        Utils_writeI2cReg(i2cFileDesc_BUS1, REG_OUTB2, 0x00);
        Utils_writeI2cReg(i2cFileDesc_BUS1, REG_OUTC2, 0x00);
        Utils_writeI2cReg(i2cFileDesc_BUS1, REG_OUTD2, 0x00);

        pthread_create(&seg_display_thread, NULL, seg_display_thread_helper, NULL);
    }
}

void Seg_stop_thread(void) {
    running = false;
    pthread_join(seg_display_thread, NULL);
    Utils_writeI2cReg(i2cFileDesc_BUS1, REG_DISPLAY_OFF, 0xFF);
    close(i2cFileDesc_BUS1);
}