// udp_socket.h
// Module containing a socket that listens for commands
// For testing: netcat -u 192.168.7.2 {PORT}
#ifndef _UDP_SOCKET_H_
#define _UDP_SOCKET_H_

#include <netdb.h>

/**
 * Starts the UDP socket thread
 *
 * @param port an int representing the port to connect on
 */
void UDP_start_listening(int port);

// stops the UDP socket thread
void UDP_stop_listening(void);

/**
 * Send a message to the web client using the server
 *
 * @param http_message message to send to the web client
 * @param sin_remote a pointer referencing the client to send a message to
 */
void UDP_send(char* http_message, struct sockaddr_in *sin_remote);

/**
 * Send a message to the default web client using the server
 *
 * @param http_message message to send to the web client
 */
void UDP_send_default(char* http_message);

// Get an int representing the maximum sized message for UDP
int UDP_get_msg_max_len(void);
#endif
