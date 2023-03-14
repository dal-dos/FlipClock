// server.h
// module that contains methods to communicate with the web client
#ifndef _SERVER_H_
#define _SERVER_H_

#include <netdb.h>

/**
 * Starts the socket with the following params
 *
 * @param port an int representing the port to be used by the server
 */
void Server_start_server(int port);

// stops the server
void Server_stop_server();

/**
 * Send a message to the web client using the server
 *
 * @param http_message message to send to the web client
 * @param sin_remote a pointer referencing the client to send a message to
 */
void Server_send(char* http_message, struct sockaddr *sin_remote);

#endif
