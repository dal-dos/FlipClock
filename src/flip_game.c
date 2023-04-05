// flip_game.c
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#include "flip_game.h"
#include "accelerometer.h"
#include "audio_mixer.h"
#include "utils.h"

// static pthread_t flip_thread;
// static bool running = false;

#define MAX_FLIPS 6
#define MIN_FLIPS 3

#define RED_FILE "audio_files/red.wav"
#define GREEN_FILE "audio_files/green.wav"
#define BLUE_FILE "audio_files/blue.wav"
#define BLACK_FILE "audio_files/black.wav"
#define PURPLE_FILE "audio_files/purple.wav"
#define YELLOW_FILE "audio_files/yellow.wav"

// static void* flip_thread_helper(void* arg) {

// }

// void Flip_start_thread(void) {
//     if (running != true) {
//         running = true;
//         pthread_create(&flip_thread, NULL, flip_thread_helper, NULL);
//     }
// }

// void Flip_stop_thread(void) {
//     running = false;
//     pthread_join(flip_thread, NULL);
// }

bool Flip_start_game(long long timeout, bool* early_stop) {
    Accel_direction flip_values[MAX_FLIPS] = {NONE};
    srand(time(NULL));
    // generate values
    int flip_count = MIN_FLIPS + rand() % (MAX_FLIPS+1 - MIN_FLIPS);
    for (int i = 0; i < flip_count; i++) {
        while (1) {
            if (i == 0) {
                Accel_direction dir = rand() % (LENGTH-1);
                if (dir != Accel_get_recent_trig()) { // don't select the starting position as the first flip
                    flip_values[i] = dir;
                    break;
                }
            } else {
                Accel_direction dir = rand() % (LENGTH-1);
                if (dir != flip_values[i-1]) { // don't allow dupes
                    flip_values[i] = dir;
                    break;
                }
            }
        }
    }

    long long start_time = Utils_get_time_in_ms();
    wavedata_t red_sound, green_sound, blue_sound, black_sound, purple_sound, yellow_sound;
    AudioMixer_readWaveFileIntoMemory(RED_FILE, &red_sound);
    AudioMixer_readWaveFileIntoMemory(GREEN_FILE, &green_sound);
    AudioMixer_readWaveFileIntoMemory(BLUE_FILE, &blue_sound);
    AudioMixer_readWaveFileIntoMemory(BLACK_FILE, &black_sound);
    AudioMixer_readWaveFileIntoMemory(PURPLE_FILE, &purple_sound);
    AudioMixer_readWaveFileIntoMemory(YELLOW_FILE, &yellow_sound);

    // start checking
    for (int i = 0; i < flip_count; i++) {
        long long time_elapsed = 0;
        // replace with LED indicators
        wavedata_t* current_sound;
        switch(flip_values[i]) {
            case 0:
                printf("RIGHT\n");
                current_sound = &purple_sound;
                break;
            case 1:
                printf("LEFT\n");
                current_sound = &yellow_sound;
                break;
            case 2:
                printf("FRONT\n");
                current_sound = &blue_sound;
                break;
            case 3:
                printf("BACK\n");
                current_sound = &black_sound;
                break;
            case 4:
                printf("TOP\n");
                current_sound = &red_sound;
                break;
            case 5:
                printf("BOTTOM\n");
                current_sound = &green_sound;
                break;
            case 6:
                break;
            case 7:
                break;
        }

        int itr_count = 0;
        const int sound_interval = 2000; // ms
        const int sleep_for = 100;
        while (Accel_get_recent_trig() != flip_values[i]) {
            if (itr_count % (sound_interval/sleep_for) == 0) { // only queue sound every x intervals
                AudioMixer_queueSound(current_sound);
            }
            itr_count++;
            time_elapsed = Utils_get_time_in_ms()-start_time;
            if (time_elapsed >= timeout) { break; }
            if (!(*early_stop)) { break; }
            Utils_sleep_for_ms(sleep_for);
        }
        if (time_elapsed >= timeout) { return false; }
        if (!(*early_stop)) { break; }
        printf("*\n");
        Utils_sleep_for_ms(1000);
    }
    return true;
}