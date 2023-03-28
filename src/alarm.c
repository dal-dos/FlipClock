// alarm.c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#include "alarm.h"
#include "time_utils.h"
#include "utils.h"
#include "audio_mixer.h"

#define C_SHARP_5_FILE "audio_files/C_SHARP_5.wav"

static pthread_t alarm_thread;
static bool running = false;

static bool armed = false; // whether or not the alarm is ready to be triggered
static bool triggered = false; // if the alarm sound is currently playing
static bool dismissed = false;

static clock_time trigger_time = { 0, 0, 0 };
static unsigned long long timeout = 3*60*1000; // default timeout = 3 minutes

static wavedata_t current_sound;

static void* alarm_thread_helper(void* arg) {
    while (running) {
        if (armed) {
            clock_time current_time = Clock_get_time();
            if ((current_time.hours == trigger_time.hours) // Alarm triggered
                    && (current_time.minutes == trigger_time.minutes)
                    && (current_time.seconds == trigger_time.seconds)) { // only trigger at the start of the minute
                long long start_time = Utils_get_time_in_ms();
                triggered = true;
                int itr_count = 0;
                const int sound_interval = 1000; // ms
                const int sleep_for = 100;

                printf("[Alarm started]\n");
                while (((timeout > 0) && (Utils_get_time_in_ms()-start_time) < timeout) && !dismissed && running) {
                    // play alarm sounds here
                    if (itr_count % (sound_interval/sleep_for) == 0) { // only queue sound every x intervals
                        AudioMixer_queueSound(&current_sound);
                    }
                    itr_count++;

                    Utils_sleep_for_ms(sleep_for);
                }
                printf("[Alarm stopped]\n");
                dismissed = false;
                triggered = false;
            }
        }
        Utils_sleep_for_ms(250);
    }
    return NULL;
}

void Alarm_start_thread(void) {
    if (running != true) {
        AudioMixer_readWaveFileIntoMemory(C_SHARP_5_FILE, &current_sound);
        running = true;
        pthread_create(&alarm_thread, NULL, alarm_thread_helper, NULL);
    }
}

void Alarm_stop_thread(void) {
    running = false;
    pthread_join(alarm_thread, NULL);
}

void Alarm_set_trigger_time(unsigned int hours, unsigned int minutes) {
    trigger_time.hours = (hours >= 24) ? 0 : hours;
    trigger_time.minutes = (minutes >= 60) ? 0 : minutes;
    armed = true;
}

void Alarm_set_timeout(unsigned int dismiss_after) {
    timeout = dismiss_after*60*1000;
}

void Alarm_disable_alarm(void) {
    armed = false;
}

void Alarm_dismiss(void) {
    dismissed = true;
}

clock_time Alarm_get_trigger_time(void) {
    return trigger_time;
}

unsigned long long Alarm_get_timeout(void) {
    return timeout;
}

bool Alarm_get_triggered(void) {
    return triggered;
}