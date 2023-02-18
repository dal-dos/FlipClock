// utils.c
// contains helper functions such as sleep
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>

#include "utils.h"

// Taken from A1
void Utils_sleep_for_ms(long long delayInMs) {
    const long long NS_PER_MS = 1000 * 1000;
    const long long NS_PER_SECOND = 1000000000;
    long long delayNs = delayInMs * NS_PER_MS;
    int seconds = delayNs / NS_PER_SECOND;
    int nanoseconds = delayNs % NS_PER_SECOND;
    struct timespec reqDelay = {seconds, nanoseconds};
    nanosleep(&reqDelay, (struct timespec *) NULL);
}

int Utils_get_voltage_reading(char* file_path) {
    // Open file
    FILE *f = fopen(file_path, "r");
    if (!f) {
        printf("ERROR: Unable to open voltage input file. Cape loaded?\n");
        printf(" Check /boot/uEnv.txt for correct options.\n");
        exit(-1);
    }

    // Get reading
    int a2dReading = 0;
    int itemsRead = fscanf(f, "%d", &a2dReading);
        if (itemsRead <= 0) {
        printf("ERROR: Unable to read values from voltage input file.\n");
        exit(-1);
    }

    // Close file
    fclose(f);
    return a2dReading;
}

double Utils_digital_to_volts(double reading, double max_reading, double max_volts) {
    return (reading/max_reading)*max_volts;
}

void Utils_write_int_to_file(char* file_path, int val) {
    // modified from LEDGuide.pdf
    FILE *file = fopen(file_path, "w");

    if (file == NULL) {
        printf("ERROR OPENING %s.", file_path);
        exit(1);
    }

    int charWritten = fprintf(file, "%d", val);
    if (charWritten <= 0) {
        printf("ERROR WRITING DATA");
        exit(1);
    }
    fclose(file);
}

void Utils_run_command(char* command) {
    // Execute the shell command (output into pipe)
    FILE *pipe = popen(command, "r");

    // Ignore output of the command; but consume it
    // so we don't get an error when closing the pipe.
    char buffer[1024];
    while (!feof(pipe) && !ferror(pipe)) {
        if (fgets(buffer, sizeof(buffer), pipe) == NULL)
            break;
        // printf("--> %s", buffer); // Uncomment for debugging
    }

    // Get the exit code from the pipe; non-zero is an error:
    int exitCode = WEXITSTATUS(pclose(pipe));
    if (exitCode != 0) {
        perror("Unable to execute command:");
        printf(" command: %s\n", command);
        printf(" exit code: %d\n", exitCode);
    }
}

void Utils_write_to_file(char* file_path, char* val) {
    FILE *file = fopen(file_path, "w");

    if (file == NULL) {
        printf("ERROR OPENING %s.", file_path);
        exit(1);
    }

    int charWritten = fprintf(file, val);
    if (charWritten <= 0) {
        printf("ERROR WRITING DATA");
        exit(1);
    }
    fclose(file);
}
