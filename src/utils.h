// utils.h
#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdbool.h>

// struct to pass data to thread;
typedef struct {
    bool flag;
} Utils_thread_data;

// Sleep for ms
void Utils_sleep_for_ms(long long delayInMs);

/**
 * Modified from A2DGuide.pdf
 * Function that reads a value from a provided file and returns
 * the A2D value
 *
 * @param file_path a string containing the file path to open
 * @return an int representing the digital value from A2D
 */
int Utils_get_voltage_reading(char* file_path);

/**
 * Converts the provided digital reading to volts
 *
 * @param reading a double representing the digital reading to convert to volts
 * @param max_reading a double representing the max digital reading
 * @param max_volts a double of the reference voltage value
 * @return an double representing the reading converted to volts
 */
double Utils_digital_to_volts(double reading, double max_reading, double max_volts);

/**
 * Function that writes the provided integer value to the provided file path
 *
 * @param file_path a string containing the file path to open
 * @param val an int representing the value to write
 */
void Utils_write_int_to_file(char* file_path, int val);

// Runs the provided shell command
void Utils_run_command(char* command);

/**
 * Function that writes the provided string value to the provided file path
 *
 * @param file_path a string containing the file path to open
 * @param val a string representing the value to write
 */
void Utils_write_to_file(char* file_path, char* val);

/**
 * Initializes the I2C device
 *
 * @param bus a string representing the filepath of the I2C bus to use
 * @param address an int representing the I2C device address
 * @return an int representing the file descriptor of the I2C device
 */
int Utils_initI2cBus(char* bus, int address);

/**
 * Writes to the specified I2C register
 *
 * @param i2cFileDesc an int representing the file descriptor of the I2C device to write to
 * @param regAddr a char representing the address of the register to write to
 * @param value a char representing the value to write to the i2c register
 */
void Utils_writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value);

/**
 * Reads multiple bytes from the specified I2C register
 *
 * @param i2cFileDesc an int representing the file descriptor of the I2C device to read from
 * @param regAddr a char representing the address of the register to start reading from
 * @param buff a char buffer representing the destination to read the register values into
 * @param buff_len an int representing the number of bytes to read
 */
void Utils_readMultipleI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char* buff, int buff_len);

#endif
